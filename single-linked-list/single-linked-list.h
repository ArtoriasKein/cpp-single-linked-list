#pragma once
#include <utility>
#include <new>

#include <iterator>

template <typename Type>
class SingleLinkedList {

    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

    template <typename ValueType>
    class BasicIterator {

        friend class SingleLinkedList;

        explicit BasicIterator(Node* node) {
            node_ = node;
        }

    public:

        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;
        BasicIterator(const BasicIterator<Type>& other) noexcept {
            if (other.node_ == nullptr) {
                this->node_ = nullptr;
            }
            else {
                this->node_ = other.node_;
            }
        }

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return rhs.node_ == this->node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return !(*this == rhs);
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return rhs.node_ == this->node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return !(*this == rhs);
        }

        BasicIterator& operator++() noexcept {
            if (node_ != nullptr) {
                node_ = node_->next_node;
            }
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            auto old_value(*this);
            ++(*this);
            return old_value;
        }

        [[nodiscard]] reference operator*() const noexcept {
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
            return &node_->value;
        }

    private:
        Node* node_ = nullptr;
    };

public:

    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    SingleLinkedList() = default;

    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator(&head_);
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator(const_cast<Node*>(&head_));
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator(const_cast<Node*>(&head_));
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        Node* new_node = new Node(value, pos.node_->next_node);
        pos.node_->next_node = new_node;
        ++size_;
        return Iterator(new_node);
    }

    [[nodiscard]] Iterator begin() noexcept {
        return Iterator(head_.next_node);
    }

    void PopFront() noexcept {
        if (GetSize() > 0) {
            Node* new_node = head_.next_node->next_node;
            delete head_.next_node;
            head_.next_node = new_node;
            --size_;
        }
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
        Node* delete_node = pos.node_->next_node;
        pos.node_->next_node = pos.node_->next_node->next_node;
        delete delete_node;
        --size_;
        return Iterator(pos.node_->next_node);
    }

    [[nodiscard]] Iterator end() noexcept {
        if (GetSize() > 0) {
            Node* prev_node = head_.next_node;
            for (int i = 0; i < GetSize(); ++i) {
                prev_node = prev_node->next_node;
                if (prev_node == nullptr) {
                    return Iterator(prev_node);
                }
            }
        }
        return Iterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        if (GetSize() > 0) {
            Node* prev_node = head_.next_node;
            for (int i = 0; i < GetSize(); ++i) {
                prev_node = prev_node->next_node;
                if (prev_node == nullptr) {
                    return ConstIterator(prev_node);
                }
            }
        }
        return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator(this->end());
    }

    SingleLinkedList(std::initializer_list<Type> values) {
        for (auto it = rbegin(values); it < rend(values); ++it) {
            PushFront(*it);
        }
    }

    SingleLinkedList(const SingleLinkedList& other) {
        if (size_ == 0 && head_.next_node == nullptr) {
            SingleLinkedList tmp;
            if (other.head_.next_node == nullptr) {
                swap(tmp);
            }
            else {
                for (auto& item : other) {
                    tmp.PushFront(item);
                }
                tmp.size_ = other.size_;
                for (auto& item : tmp) {
                    PushFront(item);
                }
                size_ = tmp.size_;
            }
        }
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (this->head_.next_node != rhs.head_.next_node) {
            SingleLinkedList tmp(rhs);
            swap(tmp);
        }
        return *this;
    }

    void swap(SingleLinkedList& other) noexcept {
        using std::swap;
        swap(head_.next_node, other.head_.next_node);
        swap(size_, other.size_);
    }

    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    void Clear() noexcept {
        while (size_ != 0) {
            Node* delete_node = head_.next_node;
            head_.next_node = delete_node->next_node;
            delete delete_node;
            --size_;
        }
    }

    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return (!GetSize());
    }

    ~SingleLinkedList() {
        Clear();
    }

private:
    Node head_ = Node();
    size_t size_ = 0;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(rhs < lhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (rhs < lhs);
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
}
