#include "fatfs.h"

#include "io.h"
#include "pool.h"
#include "sdhost.h"
#include "vfs.h"

int fat_base = 2048;
int sectors_per_cluster;
int first_data_sector;
struct file_operations* fatfs_f_ops;
struct vnode_operations* fatfs_v_ops;

void init_fatentry(struct fatentry* fatentry, struct vnode* vnode,
                   const char* name) {
    fatentry->vnode = vnode;
    strcpy(fatentry->name, name);
}

int get_first_sector(int cluster) {
    return ((cluster - 2) * sectors_per_cluster) + first_data_sector;
}

int fatfs_mount(struct filesystem* fs, struct mount* mount) {
    struct fat_BS* fat_boot = kmalloc(sizeof(struct fat_BS));
    readblock(fat_base, (char*)fat_boot);

    int fat_size = fat_boot->table_size_32;
    int root_dir_sectors = 0;
    sectors_per_cluster = fat_boot->sectors_per_cluster;
    first_data_sector = fat_boot->reserved_sector_count +
                        (fat_boot->table_count * fat_size) + root_dir_sectors;

    unsigned int root_cluster_32 = fat_boot->root_cluster;

    int first_sector_of_cluster = get_first_sector(root_cluster_32);

    int root_sector = first_sector_of_cluster + fat_base;

    struct fat_root* fat_root = kmalloc(sizeof(char) * 512);
    readblock(root_sector, (char*)fat_root);

    mount->fs = fs;
    struct fatentry* fatentry =
        (struct fatentry*)kmalloc(sizeof(struct fatentry));
    struct vnode* vnode = (struct vnode*)kmalloc(sizeof(struct vnode));
    vnode->v_ops = fatfs_v_ops;
    vnode->f_ops = fatfs_f_ops;
    vnode->internal = fatentry;

    fatentry->type = FILE_TYPE_D;
    fatentry->cluster_id = root_sector;

    init_fatentry(fatentry, vnode, "/");
    for (int i = 0; i < DIR_MAX; i++) {
        fatentry->list[i] = (struct fatentry*)kmalloc(sizeof(struct fatentry));
        fatentry->list[i]->parent = fatentry;
        if ((fat_root + i)->filename[0]) {
            strncpy(fatentry->list[i]->name, (fat_root + i)->filename, 8);
            size_t len = strlen(fatentry->list[i]->name);
            *(fatentry->list[i]->name + len) = '.';
            strncpy(fatentry->list[i]->name + len + 1,
                    (fat_root + i)->extension, 3);

            fatentry->list[i]->cluster_id =
                ((fat_root + i)->cluster_h << 2) + (fat_root + i)->cluster_l;

            struct vnode* vnode_child =
                (struct vnode*)kmalloc(sizeof(struct vnode));
            fatentry->list[i]->vnode = vnode_child;
            vnode_child->internal = fatentry->list[i];
            vnode_child->f_ops = vnode->f_ops;
            vnode_child->v_ops = vnode->v_ops;
            print_s(fatentry->list[i]->name);
            print_s("\n");
        } else {
            fatentry->list[i]->type = FILE_TYPE_N;
            fatentry->list[i]->name[0] = 0;
        }
    }
    mount->root = vnode;
}

int fatfs_lookup(struct vnode* dir_node, struct vnode** target,
                 const char* component_name) {
    for (int i = 0; i < DIR_MAX; i++) {
        if (!strcmp(((struct fatentry*)dir_node->internal)->list[i]->name,
                    component_name)) {
            struct fatentry* fatentry =
                ((struct fatentry*)dir_node->internal)->list[i];
            *target = fatentry->vnode;
            return 1;
        }
    }
    return -1;
}
int fatfs_write(struct file* file, const void* buf, size_t len);
int fatfs_read(struct file* file, void* buf, size_t len) {
    int index = get_first_sector(
                    ((struct fatentry*)file->vnode->internal)->cluster_id) +
                fat_base;
    print_i(index);
    readblock(index, buf);
    // struct fat_root* fat_root = kmalloc(sizeof(char) * 512);
}
int fatfs_list(struct file* file);
