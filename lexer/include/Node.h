/**
    �ڵ���, ����DFA��NFA�еĽڵ���Ϣ
*/

#ifndef REGNODE_H
#define REGNODE_H

#include "../base.h"

class Node {

public:
    Node():_is_end(false){};
    Node(bool is_end):_is_end(is_end){};

    // ����һ����
    void insert_path(Node* const next_node, const char path_char = PATH_EPSILON);
    // ����һ���
    void set_path(Node* const next_node, const char path_char);
    // ɾ��һ���
    void erase_path(const char path_char);

    bool is_end_node() const { return _is_end; }
    void set_end(bool is_end) { _is_end = is_end; }

    // ����һ���㼯, �㼯�����е��ܴ�����ڵ㿪ʼ, ͨ��path_char����ܷ��ʵ�
    void next(char const path_char, set<Node *> &node_set, bool clear_set) const;

    // ���شӵ�ǰ�ڵ�ͨ��path_char�ܷ��ʵ�������һ���ڵ�
    // ���������, ����null
    Node* const next(char path_char) const;


private:
    // ��������ڵ�ı߼�, ��Ϊ��NFA��ͬһ���ַ�����ı�(һ���)�����ж���, ������multimap������map
    multimap<char, Node *> _path_map;

    // ��ǰ�ڵ��Ƿ�Ϊ�ս�ڵ�.
    bool _is_end;
};

#endif // REGNODE_H
