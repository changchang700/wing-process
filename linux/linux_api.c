#include "linux_api.h"
extern char* PHP_PATH;
/**
 * linux或者mac查找命令所在路径，使用完需要free释放资源
 * 如：get_command_path("php"); //返回 /usr/bin/php
 *
 * @param const char* command 需要查询的命令
 * @return char* 堆内存，需要手动free，如：
 *    char *path =  wing_get_command_path("php");
 *    ...//其他操作
 *    free(path);
 */
char* wing_get_command_path(const char* command)
{
    char *env           = getenv("PATH");
    char *start         = env;
    size_t len          = strlen(env);
    char *pos           = env;
    ulong size          = 0;
    char temp[MAX_PATH] = {0};
    char *res           = NULL;
    ulong command_len   = strlen(command)+1;

    while (1) {
        if (*start == ':' ) {
            size = start - pos;
            memset(temp, 0, MAX_PATH);
            strncpy(temp, (char*)pos, size);
            char *base = (char*)(temp + strlen(temp));
            strcpy(base, "/");
            strcpy((char*)(base + 1), command);

            if (access(temp, F_OK) == 0) {
                res = (char *)malloc(size+command_len);
                memset(res, 0, size+command_len);
                strcpy(res, temp);
                return res;
            }

            pos = start+1;
        }

        if (start >= (env+len) ) {
            break;
        }

        start++;
    }

    size = env+len - pos;
    memset(temp, 0, MAX_PATH);
    strncpy(temp, (char*)pos, size);

    char *base = (char*)(temp + strlen(temp));
    strcpy(base, "/");
    strcpy((char*)(base + 1), command);

    if (access(temp, F_OK) == 0) {
        res = (char *)malloc(size+command_len);
        memset(res, 0, size+command_len);
        strcpy(res, temp);
        return res;
    }
    return NULL;
}

/**
 * 启用守护进程模式
 *
 * @param const char* dir 工作目录
 */
void init_daemon(const char* dir)
{
    int pid = fork();
    int i;
    if (pid > 0) {
        exit(0);//是父进程，结束父进程
    }
    if (pid < 0) {
        exit(1);//fork失败，退出
    }
    //是第一子进程，后台继续执行
    setsid();//第一子进程成为新的会话组长和进程组长
    //并与控制终端分离
    pid = fork();
    if (pid > 0) {
        exit(0);//是第一子进程，结束第一子进程
    }
    if (pid < 0) {
        exit(1);//fork失败，退出
    }
    //是第二子进程，继续
    //第二子进程不再是会话组长
    //for (i = 0; i < NOFILE; ++i) {//关闭打开的文件描述符
    //    close(i);
    //}
    chdir(dir);//改变工作目录到/tmp
    umask(0);//重设文件创建掩模
    return;
}

/**
 * 创建一个新的进程
 *
 * @param const char *command 需要以守护进程运行的指令
 * @param char* output_file 输出重定向到文件，如果不等于NULL，则认为是以守护方式运行
 * @return unsigned long 进程id
 */
unsigned long wing_create_process(const char *command, char* output_file)
{
    TSRMLS_FETCH();

    int daemon = output_file == NULL ? 0 : 1;
    if (daemon) {
        const char *str = zend_get_executed_filename(TSRMLS_C);
        char find_str[] = "/";
        char *find      = strstr((const char*)str, find_str);
        char *last_pos  = NULL;
        while(find) {
            last_pos = find;
            find++;
            find = strstr((const char*)find, find_str);
        }
        char path[MAX_PATH] = {0};

        strncpy((char*)path, (const char*)str, (size_t)(last_pos-str));

        init_daemon((const char*)path);

        FILE *handle = fopen(output_file, "a+");

        if (handle) {
            fclose(handle);
            fclose((FILE*)stdout);
            stdout = fopen(output_file, "a+");

            fclose((FILE*)stderr);
            stderr = fopen(output_file, "a+");
        } else {
            php_error_docref(NULL TSRMLS_CC, E_WARNING, "无法打开文件(could not open file)：%s", output_file);
        }

    }
    pid_t childpid = fork();

    if (childpid == 0) {

        //命令解析
    	char *st = (char*)command;
    	char *et = (char*)(st + strlen(command));
    	char _args[MAX_ARGC][MAX_PATH];
    	int pos = 0;
    	int ac = 0;
    	int cc = 0;
    	int start = 0;
    	int next_s = 0;
        int na = 0;
    	int i;
    	for (i = 0; i < MAX_ARGC; i++) {
    		memset(*_args, 0, MAX_PATH);
    	}

    	//命令行参数解析算法 主要是为了解决带空格路径和带空格参数的问题
    	//可以使用 单引号 双引号 和 ` 符号包含带空格的额参数
    	//如下算法只是为了将如 "'__DIR__/1 2.php'     123     \"trertyertey\" '123456'  `23563456` \"sdfgfdgfdg\"";
    	//这样的字符串还原为正常的命令行参数
    	//如上字符出阿奴解析之后会得到一个数组 ["__DIR__/1 2.php", "123", "trertyertey", "123456", "23563456", "sdfgfdgfdg"]
    	//最多只支持7个参数 每个参数不得超过255个字符
    	while (st <= et) {
    		if (ac >= MAX_ARGC - 1) break;

    		if (*st == '\'' || *st == '"' || *st == '`') {
    			pos++;
    			st++;
    			start = 1;
    			if (pos == 2) {
    				while (*st == ' ')
    				{
    					st++; na = 1;
    				}
    				if (na) {
    					ac++;
    					printf("1ac++\r\n");
    					cc = 0;
    					na = 0;
    				}
    				if (*st == '\'' || *st == '"' || *st == '`') {
    				    st++; pos=1;
    				}
    			}
    		}

    		if (start == 0) {
    			if (*st == ' ') {
    				ac++; printf("2ac++\r\n");
    				cc = 0;
    			}

    			while (*st == ' ')
    				st++;

    			if (*st == '\'' || *st == '"' || *st == '`') {
    				pos++;
    				st++;
    				start = 1;
    			}
    		}

    		if (*st == '\0') break;
    		if (st >= et) break;
    		if (cc < MAX_PATH) {
    			_args[ac][cc] = *st;
    			cc++;
    			_args[ac][cc] = '\0';

    		}

    		if (pos == 2) {
    			pos = 0;
    			start = 0;
    		}
    		st++;
    	}
    	//命令解析--end

        if (wing_file_is_php(command)) {
            switch (ac) {
            case 0:
                if (execl(PHP_PATH, "php", _args[0], NULL) < 0) {
                    exit(0);
                }
                break;
            case 1:
                if (execl(PHP_PATH, "php", _args[0], _args[1], NULL) < 0) {
                    exit(0);
                }
                break;
            case 2:
                if (execl(PHP_PATH, "php", _args[0], _args[1], _args[2], NULL) < 0) {
                    exit(0);
                }
                break;
            case 3:
                if (execl(PHP_PATH, "php", _args[0], _args[1], _args[2], _args[3], NULL) < 0) {
                    exit(0);
                }
                break;
            case 4:
                if (execl(PHP_PATH, "php", _args[0], _args[1], _args[2], _args[3], _args[4], NULL) < 0) {
                    exit(0);
                }
                break;
            case 5:
                if (execl(PHP_PATH, "php", _args[0], _args[1], _args[2], _args[3], _args[4], _args[5], NULL) < 0) {
                    exit(0);
                }
                break;
            case 6:
                if (execl(PHP_PATH, "php", _args[0], _args[1], _args[2], _args[3], _args[4], _args[5], _args[6], NULL) < 0) {
                    exit(0);
                }
                break;
            case 7:
                if (execl(PHP_PATH, "php", _args[0], _args[1], _args[2], _args[3], _args[4], _args[5], _args[6], _args[7], NULL) < 0) {
                    exit(0);
                }
                break;
            default:
                break;
            }
        } else {
            if (execl("/bin/sh", "sh", "-c", command, NULL) < 0) {
                exit(0);
            }
        }
    } else if(childpid > 0) {
        if (daemon) {
            //如果以守护进程方式启动，则等待子进程退出，防止子进程变成僵尸进程
            int status;
            return waitpid(childpid, &status, 0);
        }
    } else {
         php_error_docref(NULL TSRMLS_CC, E_WARNING, "创建进程错误(fork a process error)");
    }
    return (unsigned long )childpid;
}

/**
 * 获取当前进程id
 *
 * @return int
 */
int wing_get_process_id()
{
    return getpid();
}

#ifdef __APPLE__
/**
 * mac下面获取进程占用内存 返回单位为k
 *
 * @param int process_id 进程id
 * @return int
 */
unsigned long wing_get_memory(int process_id)
{
    struct proc_taskallinfo info;

    int ret = proc_pidinfo(process_id, PROC_PIDTASKALLINFO, 0,
                           (void*)&info, sizeof(struct proc_taskallinfo));
    if (ret <= 0) {
        return 0;
    }
    return info.ptinfo.pti_resident_size/1024;
}
#else
/**
 * linux下面获取进程占用内存 返回单位为k
 *
 * @param int process_id 进程id
 * @return int
 */
unsigned long wing_get_memory(int process_id)
{
    char file[MAX_PATH] = {0};
    sprintf(file, "/proc/%d/status", process_id);
    FILE *sp = fopen(file, "r");
    if (!sp) {
       return 0;
    }
    char sbuffer[32] = { 0 };
    char mem[16]     = { 0 };
    char *cs         = NULL;
    int count        = 0;

    while (!feof(sp)) {
        memset(sbuffer, 0, 32);
        fgets(sbuffer, 32, sp);
        if (strncmp(sbuffer, "VmSize:", 7) == 0) {
            cs = (char*)(sbuffer + 6);
            while (*cs++ != '\n') {
                if (*cs >= 48 && *cs <= 57)
                mem[count++] = *cs;
            }
            break;
        }
    }

    fclose(sp);
    return atoi((const char*)mem);
}

#endif
/**
 * @todo 杀死进程
 * @param int process_id
 * @return int 0成功 -1失败
 */
int wing_kill(int process_id)
{
    return 0;
}

/**
 * 获取系统临时路径
 *
 * @param char *buffer 输出结果，这里的buffer使用char数组，如：
 *    char buffer[MAX_PATH];
 *    wing_get_tmp_dir(buffer);
 */
void wing_get_tmp_dir(char *buffer)
{
    const char* tmp = "/tmp";
    if(0 != access(tmp, W_OK)) {
        buffer = NULL;
        return;
    }

    const char* tmp_wing = "/tmp/wing_process";
    if (0 == access(tmp_wing, F_OK)) {
        strcpy(buffer, tmp_wing);
        return;
    }

    if (0 == mkdir(tmp_wing, 0777)) {
        strcpy(buffer, tmp_wing);
        return;
    }
    buffer = NULL;
    return;
}