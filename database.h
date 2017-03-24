/*********

**********/
#ifndef DATABASE_H_INCLUDED
#define DATABASE_H_INCLUDED

database_t *instantiate_database(char[] data_file_path, size_t entry_size);
void create_entry(database_t *database, void *entry);
bool seek_entry(database_t *database, void *entry_target, bool (*entry_comparator)(void*, void*));
void *read_entry(database_t *database, void *entry_target, bool (*entry_comparator)(void*, void*));
void update_entry(database_t *database, void *entry_target, bool (*entry_comparator)(void*, void*), void *updated_entry);
void delete_entry(database_t *database, void *entry_target, bool (*entry_comparator)(void*, void*));