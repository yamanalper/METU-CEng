#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ext2fs.h"

typedef struct {
    uint32_t timestamp;
    char action[16];
    char args[512];
    char affected_dirs[256];
    char affected_inodes[256];
} history_entry;

typedef struct {
    uint32_t inode_no;
    char name[256];
    char full_path[512];
    uint32_t parent_inode;
    struct ext2_inode inode_data;
    int is_deleted;
} file_info;

file_info files[1000];
int file_count = 0;
history_entry history_entries[1000];
int history_count = 0;

typedef struct {
    uint32_t inode_no;
    char path[512];
} inode_path_map;

inode_path_map inode_paths[1000];
int inode_path_count = 0;

void read_inode(FILE *fp, struct ext2_super_block *sb, struct ext2_block_group_descriptor *all_bgds, int inode_no, struct ext2_inode *inode, int block_size);
void print_directory(FILE *fp, FILE *out, struct ext2_super_block *sb, struct ext2_block_group_descriptor *all_bgds, struct ext2_inode *inode, int block_size, int depth);
void process_directory_block(FILE *fp, FILE *out, struct ext2_super_block *sb, struct ext2_block_group_descriptor *all_bgds, uint32_t block_no, int block_size, int depth);
void process_single_indirect(FILE *fp, FILE *out, struct ext2_super_block *sb, struct ext2_block_group_descriptor *all_bgds, uint32_t block_no, int block_size, int depth);
void process_double_indirect(FILE *fp, FILE *out, struct ext2_super_block *sb, struct ext2_block_group_descriptor *all_bgds, uint32_t block_no, int block_size, int depth);
void process_triple_indirect(FILE *fp, FILE *out, struct ext2_super_block *sb, struct ext2_block_group_descriptor *all_bgds, uint32_t block_no, int block_size, int depth);
void analyze_filesystem_for_history(FILE *fp, struct ext2_super_block *sb, struct ext2_block_group_descriptor *all_bgds, int block_size);
void collect_all_files(FILE *fp, struct ext2_super_block *sb, struct ext2_block_group_descriptor *all_bgds, struct ext2_inode *dir_inode, uint32_t dir_inode_no, const char *path, int block_size);
void analyze_directory_block_for_history(FILE *fp, struct ext2_super_block *sb, struct ext2_block_group_descriptor *all_bgds, uint32_t block_no, uint32_t dir_inode_no, const char *path, int block_size);
void analyze_single_indirect_for_history(FILE *fp, struct ext2_super_block *sb, struct ext2_block_group_descriptor *all_bgds, uint32_t block_no, uint32_t dir_inode_no, const char *path, int block_size);
void analyze_double_indirect_for_history(FILE *fp, struct ext2_super_block *sb, struct ext2_block_group_descriptor *all_bgds, uint32_t block_no, uint32_t dir_inode_no, const char *path, int block_size);
void analyze_triple_indirect_for_history(FILE *fp, struct ext2_super_block *sb, struct ext2_block_group_descriptor *all_bgds, uint32_t block_no, uint32_t dir_inode_no, const char *path, int block_size);
void generate_history();
void add_history_entry(uint32_t timestamp, const char *action, const char *args, const char *affected_dirs, const char *affected_inodes);
void write_history_output(const char *output_file);
void scan_all_inodes_for_deleted_files(FILE *fp, struct ext2_super_block *sb, struct ext2_block_group_descriptor *all_bgds, int block_size);
int find_deleted_file_path(FILE *fp, struct ext2_super_block *sb, struct ext2_block_group_descriptor *all_bgds, uint32_t target_inode, char *path, uint32_t *parent_inode, int block_size);
void build_inode_path_mapping(FILE *fp, struct ext2_super_block *sb, struct ext2_block_group_descriptor *all_bgds, struct ext2_inode *dir_inode, uint32_t dir_inode_no, const char *path, int block_size);
void build_mapping_from_block(FILE *fp, struct ext2_super_block *sb, struct ext2_block_group_descriptor *all_bgds, uint32_t block_no, const char *parent_path, int block_size);
char* get_path_from_inode(uint32_t inode_no);
int scan_block_for_deleted_entry(uint8_t *block, int block_size, uint32_t target_inode, char *path, uint32_t *parent_inode);

void scan_all_inodes_for_deleted_files(FILE *fp, struct ext2_super_block *sb, struct ext2_block_group_descriptor *all_bgds, int block_size) {
    for (uint32_t inode_no = 11; inode_no <= sb->inode_count && file_count < 1000; inode_no++) {
        struct ext2_inode inode;
        read_inode(fp, sb, all_bgds, inode_no, &inode, block_size);
        if (inode.mode == 0) continue;
        
        int found_in_dirs = 0;
        for (int i = 0; i < file_count; i++) {
            if (files[i].inode_no == inode_no) {
                found_in_dirs = 1;
                break;
            }
        }

        if (!found_in_dirs) {
            char reconstructed_path[512];
            uint32_t parent_inode = 2;
            
            if (find_deleted_file_path(fp, sb, all_bgds, inode_no, reconstructed_path, &parent_inode, block_size)) {
                
            } else {
                snprintf(reconstructed_path, sizeof(reconstructed_path), "/unknown_inode_%u", inode_no);
            }
            
            files[file_count].inode_no = inode_no;
            
            char *filename = strrchr(reconstructed_path, '/');
            if (filename) {
                strcpy(files[file_count].name, filename + 1);
            } else {
                snprintf(files[file_count].name, sizeof(files[file_count].name), "inode_%u", inode_no);
            }
            
            strcpy(files[file_count].full_path, reconstructed_path);
            files[file_count].parent_inode = parent_inode;
            files[file_count].inode_data = inode;
            files[file_count].is_deleted = 1;
            file_count++;
        }
    }
}

int find_deleted_file_path(FILE *fp, struct ext2_super_block *sb, struct ext2_block_group_descriptor *all_bgds, uint32_t target_inode, char *path, uint32_t *parent_inode, int block_size) {
    (void)all_bgds;
    
    for (uint32_t block_no = sb->first_data_block; block_no < sb->block_count; block_no++) {
        uint8_t *block = malloc(block_size);
        if (!block) continue;
        
        fseek(fp, (long)block_no * block_size, SEEK_SET);
        size_t bytes_read = fread(block, 1, block_size, fp);
        
        if (bytes_read != (size_t)block_size) {
            free(block);
            continue;
        }
        
        if (scan_block_for_deleted_entry(block, block_size, target_inode, path, parent_inode)) {
            free(block);
            return 1;
        }
        
        free(block);
    }
    
    return 0;
}

char* get_path_from_inode(uint32_t inode_no) {
    for (int i = 0; i < inode_path_count; i++) {
        if (inode_paths[i].inode_no == inode_no) {
            return inode_paths[i].path;
        }
    }
    return NULL;
}

int scan_block_for_deleted_entry(uint8_t *block, int block_size, uint32_t target_inode, char *path, uint32_t *parent_inode) {
    unsigned int offset = 0;
    uint32_t found_parent = 0;
    
    while (offset < (unsigned int)block_size) {
        if (offset + sizeof(struct ext2_dir_entry) > (unsigned int)block_size) break;
        
        struct ext2_dir_entry *entry = (struct ext2_dir_entry *)(block + offset);
        
        if (entry->length == 0 || entry->length < 8 || offset + entry->length > (unsigned int)block_size) {
            break;
        }
        
        if (entry->inode != 0 && entry->name_length == 1 && entry->name[0] == '.') {
            found_parent = entry->inode;
            break;
        }
        
        offset += entry->length;
    }
    
    char *parent_path = get_path_from_inode(found_parent);
    if (!parent_path) {
        return 0;
    }
    
    offset = 0;
    while (offset < (unsigned int)block_size) {
        if (offset + sizeof(struct ext2_dir_entry) > (unsigned int)block_size) break;
        
        struct ext2_dir_entry *entry = (struct ext2_dir_entry *)(block + offset);
        
        if (entry->length == 0 || entry->length < 8 || offset + entry->length > (unsigned int)block_size) {
            break;
        }
        
        unsigned int curr_entry_actual_size = 8 + entry->name_length;
        curr_entry_actual_size = (curr_entry_actual_size + 3) & ~3;
        
        if (entry->length > curr_entry_actual_size && offset + entry->length <= (unsigned int)block_size) {
            unsigned int ghost_scan_offset = offset + curr_entry_actual_size;
            unsigned int entry_end = offset + entry->length;
            
            while (ghost_scan_offset + 8 <= entry_end) {
                struct ext2_dir_entry *ghost_entry = (struct ext2_dir_entry *)(block + ghost_scan_offset);
                
                if (ghost_entry->inode == target_inode && 
                    ghost_entry->name_length > 0 && 
                    ghost_entry->name_length < 255 &&
                    ghost_scan_offset + 8 + ghost_entry->name_length <= entry_end) {
                    
                    char name_buf[256];
                    memcpy(name_buf, ghost_entry->name, ghost_entry->name_length);
                    name_buf[ghost_entry->name_length] = '\0';
                    
                    if (strcmp(name_buf, ".") != 0 && strcmp(name_buf, "..") != 0) {
                        snprintf(path, 512, "%s%s", parent_path, name_buf);
                        *parent_inode = found_parent;
                        return 1;
                    }
                }
                
                if (ghost_scan_offset + ghost_entry->length > entry_end || ghost_entry->length < 8) break;
                ghost_scan_offset += ghost_entry->length;
            }
        }
        
        offset += entry->length;
    }
    
    return 0;
}

int main(int argc, char *argv[]) {
    char *filesystem_image_path = argv[1];
    char *history_output_path = (argc >= 4) ? argv[3] : NULL;

    FILE *fs_file = fopen(filesystem_image_path, "rb");
    FILE *state_output = fopen(argv[2], "w");
    struct ext2_super_block sb;
    
    fseek(fs_file, 1024, SEEK_SET);
    fread(&sb, sizeof(struct ext2_super_block), 1, fs_file);
    int block_size = 1024 << sb.log_block_size;
    int bgd_block = (block_size == 1024) ? 2 : 1;
    long bgd_offset = block_size * bgd_block;
    unsigned int num_block_groups = (sb.block_count + sb.blocks_per_group - 1) / sb.blocks_per_group;
    struct ext2_block_group_descriptor *all_bgds = malloc(num_block_groups * sizeof(struct ext2_block_group_descriptor));
    fseek(fs_file, bgd_offset, SEEK_SET);
    fread(all_bgds, sizeof(struct ext2_block_group_descriptor), num_block_groups, fs_file);
    
    struct ext2_inode root_inode;
    read_inode(fs_file, &sb, all_bgds, 2, &root_inode, block_size);
    
    fprintf(state_output, "- 2:root/\n");
    print_directory(fs_file, state_output, &sb, all_bgds, &root_inode, block_size, 2);
    fclose(state_output);

    if (history_output_path) {
        analyze_filesystem_for_history(fs_file, &sb, all_bgds, block_size);
        generate_history();
        write_history_output(history_output_path);
    }

    free(all_bgds);
    fclose(fs_file);
    return 0;
}

void read_inode(FILE *fp, struct ext2_super_block *sb, struct ext2_block_group_descriptor *all_bgds, int inode_no, struct ext2_inode *inode, int block_size) {
    unsigned int inodes_per_group = sb->inodes_per_group;
    unsigned int group_no = (inode_no - 1) / inodes_per_group;
    unsigned int index_in_group = (inode_no - 1) % inodes_per_group;
    unsigned int inode_table_start_block = all_bgds[group_no].inode_table;
    unsigned long inode_offset = (unsigned long)inode_table_start_block * block_size + (unsigned long)index_in_group * sb->inode_size;
    
    uint8_t *temp = malloc(sb->inode_size);
    fseek(fp, inode_offset, SEEK_SET);
    fread(temp, 1, sb->inode_size, fp);
    memcpy(inode, temp, sizeof(struct ext2_inode));
    free(temp);
}

void print_directory(FILE *fp, FILE *out, struct ext2_super_block *sb, struct ext2_block_group_descriptor *all_bgds, struct ext2_inode *inode, int block_size, int depth) {
    for (int i = 0; i < 12; ++i) {
        if (inode->direct_blocks[i] != 0) {
            process_directory_block(fp, out, sb, all_bgds, inode->direct_blocks[i], block_size, depth);
        }
    }
    process_single_indirect(fp, out, sb, all_bgds, inode->single_indirect, block_size, depth);
    process_double_indirect(fp, out, sb, all_bgds, inode->double_indirect, block_size, depth);
    process_triple_indirect(fp, out, sb, all_bgds, inode->triple_indirect, block_size, depth);
}

void process_directory_block(FILE *fp, FILE *out, struct ext2_super_block *sb, struct ext2_block_group_descriptor *all_bgds, uint32_t block_no, int block_size, int depth) {
    if (block_no == 0) return;

    uint8_t *block = malloc(block_size);
    long seek_pos = (long)block_no * block_size;
    fseek(fp, seek_pos, SEEK_SET);
    fread(block, 1, block_size, fp);
  
    int offset = 0;
    while (offset < block_size) {
        struct ext2_dir_entry *entry = (struct ext2_dir_entry *)(block + offset);

        if (entry->inode != 0 && entry->name_length > 0) {
            char name_buf[256];
            memcpy(name_buf, entry->name, entry->name_length);
            name_buf[entry->name_length] = '\0';

            if (strcmp(name_buf, ".") != 0 && strcmp(name_buf, "..") != 0) {
                struct ext2_inode child_inode;
                read_inode(fp, sb, all_bgds, entry->inode, &child_inode, block_size);
                if (child_inode.mode != 0) { 
                    int is_dir = (child_inode.mode & 0xF000) == 0x4000;

                    for (int d = 0; d < depth; ++d) {
                        fprintf(out, "-");
                    }

                    fprintf(out, " %u:%s%s\n", entry->inode, name_buf, is_dir ? "/" : "");

                    if (is_dir) {
                        print_directory(fp, out, sb, all_bgds, &child_inode, block_size, depth + 1);
                    }
                }
            }
        }

        unsigned int curr_entry_actual_size = 8 + entry->name_length;
        curr_entry_actual_size = (curr_entry_actual_size + 3) & ~3;

        if (entry->length > curr_entry_actual_size) {
            unsigned int ghost_scan_offset = offset + curr_entry_actual_size;
            while (ghost_scan_offset < (unsigned int)(offset + entry->length)) {
                struct ext2_dir_entry *ghost_entry = (struct ext2_dir_entry *)(block + ghost_scan_offset);
                if (ghost_entry->length == 0) break;
                if (ghost_entry->inode != 0 && ghost_entry->name_length > 0) {
                    char ghost_name_buf[256];
                    memcpy(ghost_name_buf, ghost_entry->name, ghost_entry->name_length);
                    ghost_name_buf[ghost_entry->name_length] = '\0';

                    if (strcmp(ghost_name_buf, ".") != 0 && strcmp(ghost_name_buf, "..") != 0) {
                        struct ext2_inode ghost_inode_struct;
                        read_inode(fp, sb, all_bgds, ghost_entry->inode, &ghost_inode_struct, block_size);
                        int is_ghost_dir = 0;
                        if(ghost_inode_struct.mode != 0){
                             is_ghost_dir = (ghost_inode_struct.mode & 0xF000) == 0x4000;
                        }
                        for (int d = 0; d < depth; ++d) {
                            fprintf(out, "-");
                        }
                        fprintf(out, " (%u:%s%s)\n", ghost_entry->inode, ghost_name_buf, is_ghost_dir ? "/" : "");
                    }
                }
                if (ghost_scan_offset + ghost_entry->length > (unsigned int)(offset + entry->length) || ghost_entry->length < 8 ) break;
                ghost_scan_offset += ghost_entry->length;
            }
        }
        offset += entry->length;
    }

    free(block);
}

void process_single_indirect(FILE *fp, FILE *out, struct ext2_super_block *sb, struct ext2_block_group_descriptor *all_bgds, uint32_t block_no, int block_size, int depth) {
    if (block_no == 0) return;

    uint32_t *indirect = malloc(block_size);
    fseek(fp, (long)block_no * block_size, SEEK_SET);
    fread(indirect, block_size, 1, fp);

    int entries = block_size / sizeof(uint32_t);
    for (int i = 0; i < entries; ++i) {
        if (indirect[i] != 0) {
            process_directory_block(fp, out, sb, all_bgds, indirect[i], block_size, depth);
        }
    }

    free(indirect);
}

void process_double_indirect(FILE *fp, FILE *out, struct ext2_super_block *sb, struct ext2_block_group_descriptor *all_bgds, uint32_t block_no, int block_size, int depth) {
    if (block_no == 0) return;

    uint32_t *dbl = malloc(block_size);
    fseek(fp, (long)block_no * block_size, SEEK_SET);
    fread(dbl, block_size, 1, fp);

    int entries = block_size / sizeof(uint32_t);
    for (int i = 0; i < entries; ++i) {
        if (dbl[i] != 0) {
            process_single_indirect(fp, out, sb, all_bgds, dbl[i], block_size, depth);
        }
    }

    free(dbl);
}

void process_triple_indirect(FILE *fp, FILE *out, struct ext2_super_block *sb, struct ext2_block_group_descriptor *all_bgds, uint32_t block_no, int block_size, int depth) {
    if (block_no == 0) return;

    uint32_t *trl = malloc(block_size);
    fseek(fp, (long)block_no * block_size, SEEK_SET);
    fread(trl, block_size, 1, fp);

    int entries = block_size / sizeof(uint32_t);
    for (int i = 0; i < entries; ++i) {
        if (trl[i] != 0) {
            process_double_indirect(fp, out, sb, all_bgds, trl[i], block_size, depth);
        }
    }

    free(trl);
}

void build_inode_path_mapping(FILE *fp, struct ext2_super_block *sb, struct ext2_block_group_descriptor *all_bgds, struct ext2_inode *dir_inode, uint32_t dir_inode_no, const char *path, int block_size) {
    int already_exists = 0;
    for (int i = 0; i < inode_path_count; i++) {
        if (inode_paths[i].inode_no == dir_inode_no) {
            already_exists = 1;
            break;
        }
    }
    
    if (!already_exists && inode_path_count < 1000) {
        inode_paths[inode_path_count].inode_no = dir_inode_no;
        strcpy(inode_paths[inode_path_count].path, path);
        inode_path_count++;
    }
    
    for (int i = 0; i < 12; ++i) {
        if (dir_inode->direct_blocks[i] != 0) {
            build_mapping_from_block(fp, sb, all_bgds, dir_inode->direct_blocks[i], path, block_size);
        }
    }
}

void build_mapping_from_block(FILE *fp, struct ext2_super_block *sb, struct ext2_block_group_descriptor *all_bgds, uint32_t block_no, const char *parent_path, int block_size) {
    if (block_no == 0) return;

    uint8_t *block = malloc(block_size);
    if (!block) return;

    fseek(fp, (long)block_no * block_size, SEEK_SET);
    size_t bytes_read = fread(block, 1, block_size, fp);
    
    if (bytes_read != (size_t)block_size) {
        free(block);
        return;
    }

    unsigned int offset = 0;
    while (offset < (unsigned int)block_size && inode_path_count < 1000) {
        if (offset + sizeof(struct ext2_dir_entry) > (unsigned int)block_size) break;
        
        struct ext2_dir_entry *entry = (struct ext2_dir_entry *)(block + offset);
        
        if (entry->length == 0 || entry->length < 8 || offset + entry->length > (unsigned int)block_size) {
            break;
        }

        if (entry->inode != 0 && entry->name_length > 0) {
            char name_buf[256];
            memcpy(name_buf, entry->name, entry->name_length);
            name_buf[entry->name_length] = '\0';

            if (strcmp(name_buf, ".") != 0 && strcmp(name_buf, "..") != 0) {
                struct ext2_inode file_inode;
                read_inode(fp, sb, all_bgds, entry->inode, &file_inode, block_size);
                
                if (file_inode.mode != 0) {
                    char full_path[512];
                    snprintf(full_path, sizeof(full_path), "%s%s", parent_path, name_buf);
                    
                    int already_exists = 0;
                    for (int i = 0; i < inode_path_count; i++) {
                        if (inode_paths[i].inode_no == entry->inode) {
                            already_exists = 1;
                            break;
                        }
                    }
                    
                    if (!already_exists && inode_path_count < 1000) {
                        inode_paths[inode_path_count].inode_no = entry->inode;
                        strcpy(inode_paths[inode_path_count].path, full_path);
                        inode_path_count++;
                    }

                    if ((file_inode.mode & 0xF000) == 0x4000) {
                        char new_path[512];
                        snprintf(new_path, sizeof(new_path), "%s/", full_path);
                        build_inode_path_mapping(fp, sb, all_bgds, &file_inode, entry->inode, new_path, block_size);
                    }
                }
            }
        }

        unsigned int curr_entry_actual_size = 8 + entry->name_length;
        curr_entry_actual_size = (curr_entry_actual_size + 3) & ~3;

        if (entry->length > curr_entry_actual_size && offset + entry->length <= (unsigned int)block_size) {
            unsigned int ghost_scan_offset = offset + curr_entry_actual_size;
            unsigned int entry_end = offset + entry->length;
            
            while (ghost_scan_offset + 8 <= entry_end && inode_path_count < 1000) {
                struct ext2_dir_entry *ghost_entry = (struct ext2_dir_entry *)(block + ghost_scan_offset);
                
                if (ghost_entry->inode != 0 && 
                    ghost_entry->name_length > 0 && 
                    ghost_scan_offset + 8 + ghost_entry->name_length <= entry_end) {
                    
                    char ghost_name_buf[256];
                    memcpy(ghost_name_buf, ghost_entry->name, ghost_entry->name_length);
                    ghost_name_buf[ghost_entry->name_length] = '\0';

                    if (strcmp(ghost_name_buf, ".") != 0 && strcmp(ghost_name_buf, "..") != 0) {
                        struct ext2_inode ghost_inode;
                        read_inode(fp, sb, all_bgds, ghost_entry->inode, &ghost_inode, block_size);
                        
                        if (ghost_inode.mode != 0) {
                            char full_path[512];
                            snprintf(full_path, sizeof(full_path), "%s%s", parent_path, ghost_name_buf);
                            
                            int already_exists = 0;
                            for (int i = 0; i < inode_path_count; i++) {
                                if (inode_paths[i].inode_no == ghost_entry->inode) {
                                    already_exists = 1;
                                    break;
                                }
                            }
                            
                            if (!already_exists && inode_path_count < 1000) {
                                inode_paths[inode_path_count].inode_no = ghost_entry->inode;
                                
                                if ((ghost_inode.mode & 0xF000) == 0x4000) {
                                    snprintf(inode_paths[inode_path_count].path, sizeof(inode_paths[inode_path_count].path), 
                                             "%s/", full_path);
                                } else {
                                    strcpy(inode_paths[inode_path_count].path, full_path);
                                }
                                
                                inode_path_count++;
                            }

                            if ((ghost_inode.mode & 0xF000) == 0x4000) {
                                char new_path[512];
                                snprintf(new_path, sizeof(new_path), "%s/", full_path);
                                build_inode_path_mapping(fp, sb, all_bgds, &ghost_inode, ghost_entry->inode, new_path, block_size);
                            }
                        }
                    }
                }
                
                if (ghost_scan_offset + ghost_entry->length > entry_end || ghost_entry->length < 8) break;
                ghost_scan_offset += ghost_entry->length;
            }
        }
        
        offset += entry->length;
    }

    free(block);
}

void analyze_filesystem_for_history(FILE *fp, struct ext2_super_block *sb, struct ext2_block_group_descriptor *all_bgds, int block_size) {
    file_count = 0;
    inode_path_count = 0;
    
    struct ext2_inode root_inode;
    read_inode(fp, sb, all_bgds, 2, &root_inode, block_size);
    
    inode_paths[0].inode_no = 2;
    strcpy(inode_paths[0].path, "/");
    inode_path_count = 1;
    
    build_inode_path_mapping(fp, sb, all_bgds, &root_inode, 2, "/", block_size);
    
    collect_all_files(fp, sb, all_bgds, &root_inode, 2, "/", block_size);
    
    scan_all_inodes_for_deleted_files(fp, sb, all_bgds, block_size);
}

void generate_history() {
    history_count = 0;
    
    for (int i = 0; i < file_count - 1; i++) {
        for (int j = i + 1; j < file_count; j++) {
            if (files[i].inode_no == files[j].inode_no && 
                strcmp(files[i].full_path, files[j].full_path) == 0 &&
                files[i].is_deleted == files[j].is_deleted) {
                for (int k = j; k < file_count - 1; k++) {
                    files[k] = files[k + 1];
                }
                file_count--;
                j--;
            }
        }
    }

    for (int i = 0; i < file_count && history_count < 1000; i++) {
        file_info *file = &files[i];
        
        if (!file->is_deleted) {
            int was_moved = 0;
            for (int j = 0; j < file_count; j++) {
                if (i != j && files[j].inode_no == file->inode_no && files[j].is_deleted) {
                    was_moved = 1;
                    break;
                }
            }
            
            if (!was_moved) {
                char args[512];
                char affected_dirs[256];
                char affected_inodes[256];
                
                snprintf(args, sizeof(args), "[%s]", file->full_path);
                snprintf(affected_dirs, sizeof(affected_dirs), "[%u]", file->parent_inode);
                snprintf(affected_inodes, sizeof(affected_inodes), "[%u]", file->inode_no);
                
                uint32_t creation_time = file->inode_data.access_time;
                
                if ((file->inode_data.mode & 0xF000) == 0x4000) {
                    add_history_entry(creation_time, "mkdir", args, affected_dirs, affected_inodes);
                } else {
                    add_history_entry(creation_time, "touch", args, affected_dirs, affected_inodes);
                }
            }
        }
    }
    
    for (int i = 0; i < file_count && history_count < 1000; i++) {
        file_info *file = &files[i];
        
        if (file->is_deleted) {
            int was_moved = 0;
            for (int j = 0; j < file_count; j++) {
                if (i != j && files[j].inode_no == file->inode_no && !files[j].is_deleted) {
                    was_moved = 1;
                    break;
                }
            }
            
            if (!was_moved) {
                char args[512];
                char affected_dirs[256];
                char affected_inodes[256];
                
                snprintf(args, sizeof(args), "[%s]", file->full_path);
                snprintf(affected_dirs, sizeof(affected_dirs), "[%u]", file->parent_inode);
                snprintf(affected_inodes, sizeof(affected_inodes), "[%u]", file->inode_no);
                
                uint32_t creation_time = file->inode_data.access_time;
                uint32_t deletion_time = file->inode_data.deletion_time;
                if (deletion_time == 0) deletion_time = file->inode_data.change_time;
                
                if (creation_time >= deletion_time) {
                    creation_time = deletion_time - 1;
                }
                
                if ((file->inode_data.mode & 0xF000) == 0x4000) {
                    add_history_entry(creation_time, "mkdir", args, affected_dirs, affected_inodes);
                } else {
                    add_history_entry(creation_time, "touch", args, affected_dirs, affected_inodes);
                }
            }
        }
    }
    
    for (int i = 0; i < file_count && history_count < 1000; i++) {
        for (int j = 0; j < file_count; j++) {
            if (i != j && files[i].inode_no == files[j].inode_no && 
                files[i].is_deleted && !files[j].is_deleted) {
                
                char create_args[512];
                char create_affected_dirs[256];
                char create_affected_inodes[256];
                
                snprintf(create_args, sizeof(create_args), "[%s]", files[i].full_path);
                snprintf(create_affected_dirs, sizeof(create_affected_dirs), "[%u]", files[i].parent_inode);
                snprintf(create_affected_inodes, sizeof(create_affected_inodes), "[%u]", files[i].inode_no);
                
                uint32_t creation_time = files[i].inode_data.access_time;
                
                if ((files[i].inode_data.mode & 0xF000) == 0x4000) {
                    add_history_entry(creation_time, "mkdir", create_args, create_affected_dirs, create_affected_inodes);
                } else {
                    add_history_entry(creation_time, "touch", create_args, create_affected_dirs, create_affected_inodes);
                }
                
                char mv_args[512];
                char mv_affected_dirs[256];
                char mv_affected_inodes[256];
                
                snprintf(mv_args, sizeof(mv_args), "[%s %s]", files[i].full_path, files[j].full_path);
                snprintf(mv_affected_dirs, sizeof(mv_affected_dirs), "[%u %u]", files[i].parent_inode, files[j].parent_inode);
                snprintf(mv_affected_inodes, sizeof(mv_affected_inodes), "[%u]", files[i].inode_no);
                
                uint32_t mv_time = files[j].inode_data.change_time;
                add_history_entry(mv_time, "mv", mv_args, mv_affected_dirs, mv_affected_inodes);
                
                break;
            }
        }
    }
    
    for (int i = 0; i < file_count && history_count < 1000; i++) {
        file_info *file = &files[i];
        
        if (file->is_deleted) {
            int was_moved = 0;
            for (int j = 0; j < file_count; j++) {
                if (i != j && files[j].inode_no == file->inode_no && !files[j].is_deleted) {
                    was_moved = 1;
                    break;
                }
            }
            
            if (!was_moved) {
                char args[512];
                char affected_dirs[256];
                char affected_inodes[256];
                
                snprintf(args, sizeof(args), "[%s]", file->full_path);
                snprintf(affected_dirs, sizeof(affected_dirs), "[%u]", file->parent_inode);
                snprintf(affected_inodes, sizeof(affected_inodes), "[%u]", file->inode_no);
                
                uint32_t deletion_time = file->inode_data.deletion_time;
                if (deletion_time == 0) deletion_time = file->inode_data.change_time;
                
                if ((file->inode_data.mode & 0xF000) == 0x4000) {
                    add_history_entry(deletion_time, "rmdir", args, affected_dirs, affected_inodes);
                } else {
                    add_history_entry(deletion_time, "rm", args, affected_dirs, affected_inodes);
                }
            }
        }
    }
}

void add_history_entry(uint32_t timestamp, const char *action, const char *args, const char *affected_dirs, const char *affected_inodes) {
    if (history_count < 1000) {
        history_entries[history_count].timestamp = timestamp;
        strcpy(history_entries[history_count].action, action);
        strcpy(history_entries[history_count].args, args);
        strcpy(history_entries[history_count].affected_dirs, affected_dirs);
        strcpy(history_entries[history_count].affected_inodes, affected_inodes);
        history_count++;
    }
}

void write_history_output(const char *output_file) {
    FILE *out = fopen(output_file, "w");
    
    for (int i = 0; i < history_count - 1; i++) {
        for (int j = 0; j < history_count - 1 - i; j++) {
            if (history_entries[j].timestamp > history_entries[j + 1].timestamp) {
                history_entry temp = history_entries[j];
                history_entries[j] = history_entries[j + 1];
                history_entries[j + 1] = temp;
            }
        }
    }
    
    for (int i = 0; i < history_count; i++) {
        fprintf(out, "%u %s %s %s %s\n", 
                history_entries[i].timestamp,
                history_entries[i].action,
                history_entries[i].args,
                history_entries[i].affected_dirs,
                history_entries[i].affected_inodes);
    }
    
    fclose(out);
}

void collect_all_files(FILE *fp, struct ext2_super_block *sb, struct ext2_block_group_descriptor *all_bgds, struct ext2_inode *dir_inode, uint32_t dir_inode_no, const char *path, int block_size) {
    for (int i = 0; i < 12; ++i) {
        if (dir_inode->direct_blocks[i] != 0) analyze_directory_block_for_history(fp, sb, all_bgds, dir_inode->direct_blocks[i], dir_inode_no, path, block_size);
    }
    if (dir_inode->single_indirect != 0) analyze_single_indirect_for_history(fp, sb, all_bgds, dir_inode->single_indirect, dir_inode_no, path, block_size);
    if (dir_inode->double_indirect != 0) analyze_double_indirect_for_history(fp, sb, all_bgds, dir_inode->double_indirect, dir_inode_no, path, block_size);
    if (dir_inode->triple_indirect != 0) analyze_triple_indirect_for_history(fp, sb, all_bgds, dir_inode->triple_indirect, dir_inode_no, path, block_size);
}

void analyze_directory_block_for_history(FILE *fp, struct ext2_super_block *sb, struct ext2_block_group_descriptor *all_bgds, uint32_t block_no, uint32_t dir_inode_no, const char *path, int block_size) {
    if (block_no == 0 || file_count >= 1000) return;

    uint8_t *block = malloc(block_size);
    if (!block) return;

    fseek(fp, (long)block_no * block_size, SEEK_SET);
    size_t bytes_read = fread(block, 1, block_size, fp);
    
    if (bytes_read != (size_t)block_size) {
        free(block);
        return;
    }

    unsigned int offset = 0;
    while (offset < (unsigned int)block_size && file_count < 1000) {
        if (offset + sizeof(struct ext2_dir_entry) > (unsigned int)block_size) break;
        
        struct ext2_dir_entry *entry = (struct ext2_dir_entry *)(block + offset);
        
        if (entry->length == 0 || entry->length < 8 || offset + entry->length > (unsigned int)block_size) {
            break;
        }

        if (entry->inode != 0 && entry->name_length > 0) {
            char name_buf[256];
            memcpy(name_buf, entry->name, entry->name_length);
            name_buf[entry->name_length] = '\0';

            if (strcmp(name_buf, ".") != 0 && strcmp(name_buf, "..") != 0) {
                struct ext2_inode file_inode;
                read_inode(fp, sb, all_bgds, entry->inode, &file_inode, block_size);
                
                if (file_inode.mode != 0) {
                    files[file_count].inode_no = entry->inode;
                    strcpy(files[file_count].name, name_buf);
                    snprintf(files[file_count].full_path, sizeof(files[file_count].full_path), "%s%s", path, name_buf);
                    files[file_count].parent_inode = dir_inode_no;
                    files[file_count].inode_data = file_inode;
                    files[file_count].is_deleted = 0;
                    file_count++;

                    if ((file_inode.mode & 0xF000) == 0x4000) {
                        char new_path[512];
                        snprintf(new_path, sizeof(new_path), "%s%s/", path, name_buf);
                        collect_all_files(fp, sb, all_bgds, &file_inode, entry->inode, new_path, block_size);
                    }
                }
            }
        }

        unsigned int curr_entry_actual_size = 8 + entry->name_length;
        curr_entry_actual_size = (curr_entry_actual_size + 3) & ~3;

        if (entry->length > curr_entry_actual_size && offset + entry->length <= (unsigned int)block_size) {
            unsigned int ghost_scan_offset = offset + curr_entry_actual_size;
            unsigned int entry_end = offset + entry->length;
            
            while (ghost_scan_offset + 8 <= entry_end && file_count < 1000) {
                struct ext2_dir_entry *ghost_entry = (struct ext2_dir_entry *)(block + ghost_scan_offset);
                
                if (ghost_entry->inode != 0 && 
                    ghost_entry->name_length > 0 && 
                    ghost_scan_offset + 8 + ghost_entry->name_length <= entry_end) {
                    
                    char ghost_name_buf[256];
                    memcpy(ghost_name_buf, ghost_entry->name, ghost_entry->name_length);
                    ghost_name_buf[ghost_entry->name_length] = '\0';

                    if (strcmp(ghost_name_buf, ".") != 0 && strcmp(ghost_name_buf, "..") != 0) {
                        struct ext2_inode ghost_inode;
                        read_inode(fp, sb, all_bgds, ghost_entry->inode, &ghost_inode, block_size);
                        
                        files[file_count].inode_no = ghost_entry->inode;
                        strcpy(files[file_count].name, ghost_name_buf);
                        snprintf(files[file_count].full_path, sizeof(files[file_count].full_path), "%s%s", path, ghost_name_buf);
                        files[file_count].parent_inode = dir_inode_no;
                        files[file_count].inode_data = ghost_inode;
                        files[file_count].is_deleted = 1;
                        file_count++;
                    }
                }
                
                if (ghost_scan_offset + ghost_entry->length > entry_end || ghost_entry->length < 8) break;
                ghost_scan_offset += ghost_entry->length;
            }
        }
        
        offset += entry->length;
    }

    free(block);
}

void analyze_single_indirect_for_history(FILE *fp, struct ext2_super_block *sb, struct ext2_block_group_descriptor *all_bgds, uint32_t block_no, uint32_t dir_inode_no, const char *path, int block_size) {
    if (block_no == 0 || file_count >= 1000) return;

    uint32_t *indirect = malloc(block_size);
    if (!indirect) return;

    fseek(fp, (long)block_no * block_size, SEEK_SET);
    size_t bytes_read = fread(indirect, 1, block_size, fp);
    
    if (bytes_read != (size_t)block_size) {
        free(indirect);
        return;
    }

    int entries = block_size / sizeof(uint32_t);
    for (int i = 0; i < entries && file_count < 1000; ++i) {
        if (indirect[i] != 0) {
            analyze_directory_block_for_history(fp, sb, all_bgds, indirect[i], dir_inode_no, path, block_size);
        }
    }

    free(indirect);
}

void analyze_double_indirect_for_history(FILE *fp, struct ext2_super_block *sb, struct ext2_block_group_descriptor *all_bgds, uint32_t block_no, uint32_t dir_inode_no, const char *path, int block_size) {
    if (block_no == 0 || file_count >= 1000) return;

    uint32_t *dbl = malloc(block_size);
    if (!dbl) return;

    fseek(fp, (long)block_no * block_size, SEEK_SET);
    size_t bytes_read = fread(dbl, 1, block_size, fp);
    
    if (bytes_read != (size_t)block_size) {
        free(dbl);
        return;
    }

    int entries = block_size / sizeof(uint32_t);
    for (int i = 0; i < entries && file_count < 1000; ++i) {
        if (dbl[i] != 0) {
            analyze_single_indirect_for_history(fp, sb, all_bgds, dbl[i], dir_inode_no, path, block_size);
        }
    }

    free(dbl);
}

void analyze_triple_indirect_for_history(FILE *fp, struct ext2_super_block *sb, struct ext2_block_group_descriptor *all_bgds, uint32_t block_no, uint32_t dir_inode_no, const char *path, int block_size) {
    if (block_no == 0 || file_count >= 1000) return;

    uint32_t *trl = malloc(block_size);
    fseek(fp, (long)block_no * block_size, SEEK_SET);
    fread(trl, block_size, 1, fp);

    int entries = block_size / sizeof(uint32_t);
    for (int i = 0; i < entries && file_count < 1000; ++i) {
        if (trl[i] != 0) {
            analyze_double_indirect_for_history(fp, sb, all_bgds, trl[i], dir_inode_no, path, block_size);
        }
    }

    free(trl);
}