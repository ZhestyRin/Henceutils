#include<stdio.h>
#include<sys/stat.h>
#include<syscall.h>
#include<errno.h>
#include<sys/types.h>
#include<dirent.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<pwd.h>
#include<grp.h>


void tree(const char *path, int depth, int show_all, int show_detail) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("failed");
        return;
    }

    struct dirent *entry;
    char fullpath[1024];

    while ((entry = readdir(dir)) != NULL) {
        
        if (!show_all && entry->d_name[0] == '.')
            continue;

        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        struct stat st;
        if (stat(fullpath, &st) == -1)
            continue;

        struct passwd *pw = getpwuid(st.st_uid);
        struct group  *gr = getgrgid(st.st_gid);

        
        for (int i = 0; i < depth; i++)
            printf("    ");

        
        if (show_detail) {
            printf("%04o %-8s %-8s %8ld ",
                   st.st_mode & 07777,
                   pw ? pw->pw_name : "unknown",
                   gr ? gr->gr_name : "unknown",
                   st.st_size);
        }

        printf("├── %s\n", entry->d_name);
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    const char *path = ".";
    int show_all = 0;
    int show_detail = 0;

    if (argc > 1) {
        if (strcmp(argv[1], "-a") == 0) {
            show_all = 1;
            show_detail = 1;
        } else if (strcmp(argv[1], "-l") == 0) {
            show_all = 0;
            show_detail = 0;
        } else {
            path = argv[1];
        }
    }

    if (argc > 2)
        path = argv[2];

    tree(path, 0, show_all, show_detail);
    return 0;
}

