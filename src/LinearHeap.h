#include "defines.h"

class LinearHeap
{
private:
    ul n;
    ul head_size; // max size of head table

    ul max_key; // max key value
    ul min_key; // min key value

    ul *keys; // keys of vertices
    ul *pre;  // pre array
    ul *next; // next array
    ul *head; // head table

public:
    LinearHeap(ul _n, ul K)
    {
        n = _n;
        head_size = min_key = K;
        max_key = 0;
        head = keys = pre = next = nullptr;
    }
    ~LinearHeap()
    {
        if (head != nullptr)
        {
            delete[] head;
            head = nullptr;
        }
        if (pre != nullptr)
        {
            delete[] pre;
            pre = nullptr;
        }
        if (next != nullptr)
        {
            delete[] next;
            next = nullptr;
        }
        if (keys != nullptr)
        {
            delete[] keys;
            keys = nullptr;
        }
    }

    // Initialize the data structure by (vertex id, key value) pairs
    void init(ul _n, ul K, ul *_ids, ul *_keys)
    {
        if (keys == nullptr)
            keys = new ul[n];
        if (pre == nullptr)
            pre = new ul[n];
        if (next == nullptr)
            next = new ul[n];
        if (head == nullptr)
            head = new ul[head_size + 1];

        min_key = K;
        max_key = 0;
        for (ul i = 0; i <= K; i++)
            head[i] = n;

        for (ul i = 0; i < _n; i++)
            insert(_ids[i], _keys[i]);
    }

    // Check if list is empty
    bool empty()
    {
        compress();
        return min_key > max_key;
    }

    // Insert (id, key) pair
    void insert(ul id, ul key)
    {
        keys[id] = key;
        pre[id] = n;
        next[id] = head[key];

        if (head[key] != n)
            pre[head[key]] = id;
        head[key] = id;

        if (key < min_key)
            min_key = key;
        if (key > max_key)
            max_key = key;
    }

    // Remove vertex id
    ul remove(ul id)
    {
        if (pre[id] == n)
        {
            head[keys[id]] = next[id];
            if (next[id] != n)
                pre[next[id]] = n;
        }
        else
        {
            ul pid = pre[id];
            next[pid] = next[id];
            if (next[id] != n)
                pre[next[id]] = pid;
        }
        return keys[id];
    }

    // Pop pair with maximum key value; return true if success, false otherwise
    bool extract_max(ul &id, ul &key)
    {
        if (empty())
            return false;

        id = head[max_key];
        key = max_key;
        head[max_key] = next[id];
        if (head[max_key] != n)
            pre[head[max_key]] = n;
        return true;
    }

    // Update key of vertex id by change
    ul update(ul id, ul change = 1)
    {
        ul new_key = keys[id] + change;
        remove(id);
        insert(id, new_key);
        return new_key;
    }

private:
    // Update min, max key values
    void compress()
    {
        while (min_key <= max_key && head[min_key] == n)
            min_key++;
        while (min_key <= max_key && head[max_key] == n)
            max_key--;
    }
};
