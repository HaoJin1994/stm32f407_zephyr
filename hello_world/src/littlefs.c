
#include <zephyr/fs/fs.h>
#include <zephyr/fs/littlefs.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/logging/log.h>
#include "main.h"
LOG_MODULE_REGISTER(littlefs_example, LOG_LEVEL_INF);

// 定义挂载点
FS_LITTLEFS_DECLARE_DEFAULT_CONFIG(storage);
static struct fs_mount_t lfs_mnt = {
    .type = FS_LITTLEFS,
    .fs_data = &storage,
    .storage_dev = (void *)FIXED_PARTITION_ID(slot1_partition),
    .mnt_point = "/lfs",
};

void littlefs_example(void)
{
    struct fs_file_t file;
    int rc;

    // 挂载文件系统
    rc = fs_mount(&lfs_mnt);
    if (rc != 0) {
        LOG_ERR("Failed to mount filesystem: %d", rc);
        return;
    }
    LOG_INF("Filesystem mounted at %s", lfs_mnt.mnt_point);

    // 初始化文件结构
    fs_file_t_init(&file);

    // 打开文件（创建或覆盖）
    rc = fs_open(&file, "/lfs/test.txt", FS_O_CREATE | FS_O_WRITE);
    if (rc != 0) {
        LOG_ERR("Failed to open file: %d", rc);
        return;
    }

    // 写入数据
    const char *data = "Hello, LittleFS!";
    rc = fs_write(&file, data, strlen(data));
    if (rc < 0) {
        LOG_ERR("Failed to write file: %d", rc);
        fs_close(&file);
        return;
    }
    LOG_INF("Wrote %d bytes", rc);

    // 关闭文件
    fs_close(&file);

    // 重新打开文件读取
    rc = fs_open(&file, "/lfs/test.txt", FS_O_READ);
    if (rc != 0) {
        LOG_ERR("Failed to open file for reading: %d", rc);
        return;
    }

    // 读取数据
    char read_buf[64];
    rc = fs_read(&file, read_buf, sizeof(read_buf) - 1);
    if (rc < 0) {
        LOG_ERR("Failed to read file: %d", rc);
        fs_close(&file);
        return;
    }
    read_buf[rc] = '\0';
    LOG_INF("Read: %s", read_buf);

    // 关闭文件
    fs_close(&file);

    // 列出目录内容
    struct fs_dir_t dir;
    fs_dir_t_init(&dir);

    rc = fs_opendir(&dir, "/lfs");
    if (rc != 0) {
        LOG_ERR("Failed to open directory: %d", rc);
        return;
    }

    struct fs_dirent entry;
    while (fs_readdir(&dir, &entry) == 0) {
        if (entry.name[0] == 0) {
            break;
        }
        LOG_INF("File: %s, size: %zu", entry.name, entry.size);
    }

    fs_closedir(&dir);
}
   