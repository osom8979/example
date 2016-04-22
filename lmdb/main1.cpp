
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <lmdb.h>

using namespace std;

int main(int argc, char ** argv)
{
    MDB_env * env;
    MDB_txn * txn;
    MDB_dbi dbi;

    int result = 0;

    result = mdb_env_create(&env);
    if (result != MDB_SUCCESS) {
        fprintf(stderr, "mdb_env_create() error: %d.\n", result);
        return EXIT_FAILURE;
    }
    // mdb_env_set_maxreaders(env, 1);
    // mdb_env_set_mapsize(env, 10485760);
    mdb_env_set_maxdbs(env, 1);

    result = mdb_env_open(env, "db", 0 /* MDB_FIXEDMAP | MDB_NOSYNC | MDB_RDONLY */, 0664);
    if (result != MDB_SUCCESS) {
        mdb_env_close(env);
        fprintf(stderr, "mdb_env_open() error: %d.\n", result);
        return EXIT_FAILURE;
    }

    result = mdb_txn_begin(env, NULL, 0, &txn);
    if (result != MDB_SUCCESS) {
        mdb_env_close(env);
        fprintf(stderr, "mdb_txn_begin() error: %d.\n", result);
        return EXIT_FAILURE;
    }

    char const * const DB_NAME = "first_db";
    result = mdb_dbi_open(txn, DB_NAME, MDB_CREATE, &dbi);
    if (result != MDB_SUCCESS) {
        mdb_env_close(env);
        fprintf(stderr, "mdb_dbi_open() error: %d.\n", result);
        return EXIT_FAILURE;
    }

    std::string key_str;
    std::string data_str;

    MDB_val key;
    MDB_val data;

    // 1
    key_str  = "KEY";
    data_str = "DATA";
    key.mv_size = key_str.size();
    key.mv_data = &key_str[0];
    data.mv_size = data_str.size();
    data.mv_data = &data_str[0];
    result = mdb_put(txn, dbi, &key, &data, 0);
    if (result != MDB_SUCCESS) {
        mdb_dbi_close(env, dbi);
        mdb_env_close(env);
        fprintf(stderr, "mdb_put() error: %d.\n", result);
        return EXIT_FAILURE;
    }

    // 2
    key_str  = "KEY2";
    data_str = "DATA2";
    key.mv_size = key_str.size();
    key.mv_data = &key_str[0];
    data.mv_size = data_str.size();
    data.mv_data = &data_str[0];
    result = mdb_put(txn, dbi, &key, &data, 0);
    if (result != MDB_SUCCESS) {
        mdb_dbi_close(env, dbi);
        mdb_env_close(env);
        fprintf(stderr, "mdb_put() error: %d.\n", result);
        return EXIT_FAILURE;
    }

    result = mdb_txn_commit(txn);
    if (result != MDB_SUCCESS) {
        mdb_dbi_close(env, dbi);
        mdb_env_close(env);
        fprintf(stderr, "mdb_txn_commit() error: %d.\n", result);
        return EXIT_FAILURE;
    }

    result = mdb_txn_begin(env, NULL, MDB_RDONLY, &txn);
    if (result != MDB_SUCCESS) {
        mdb_dbi_close(env, dbi);
        mdb_env_close(env);
        fprintf(stderr, "mdb_txn_begin() error: %d.\n", result);
        return EXIT_FAILURE;
    }

    MDB_cursor * cursor;
    result = mdb_cursor_open(txn, dbi, &cursor);
    while ((result = mdb_cursor_get(cursor, &key, &data, MDB_NEXT)) == 0) {
        printf("key: %p %.*s, data: %p %.*s\n",
            key.mv_data,  (int) key.mv_size,  (char *) key.mv_data,
            data.mv_data, (int) data.mv_size, (char *) data.mv_data);
    }
    mdb_cursor_close(cursor);
    mdb_txn_abort(txn);

    mdb_dbi_close(env, dbi);
    mdb_env_close(env);
    env = nullptr;

    return EXIT_SUCCESS;
}

