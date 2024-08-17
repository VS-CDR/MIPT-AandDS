#include <algorithm>
#include <array>
#include <functional>

template <typename T>
class TreeNode {
  public:
    TreeNode(T value, TreeNode* left_subtree=nullptr, TreeNode* right_subtree=nullptr) :
      value_(std::move(value)), left_subtree_(left_subtree), right_subtree_(left_subtree) {}

    [[nodiscard]] T GetValue() { return value_; }
    [[nodiscard]] TreeNode* GetLeftSubtree() { return left_subtree_; }
    [[nodiscard]] TreeNode* GetRightSubtree() { return right_subtree_; }
  
  private:
    T value_;
    TreeNode* left_subtree_;
    TreeNode* right_subtree_;
};

namespace Concepts {
template <class T>
concept TreeNode = requires(T obj) {
  obj.GetValue();
  obj.GetLeftSubtree();
  obj.GetRightSubtree();
};
}

template<typename T>
class BSTVisitor {
  static constexpr int kNumOfHelperFunctions = 3;
  public:
    BSTVisitor() {
      funcs_[0] = [this](TreeNode<T>* node) {
        VisitImpl(node->GetLeftSubtree());
      };
      funcs_[1] = [this](TreeNode<T>* node) {
        // std::cout << node->GetValue() << ' ';
      };
      funcs_[2] = [this](TreeNode<T>* node) {
        VisitImpl(node->GetRightSubtree());
      };
    }

    enum class Order { Pre, In, Post };

    void Visit(Order order, TreeNode<T>* root);

  private:
    std::array<std::function<void(TreeNode<T>*)>, kNumOfHelperFunctions> funcs_;
    void VisitImpl(TreeNode<T>* root);
};

template <typename T> void BSTVisitor<T>::Visit(Order order, TreeNode<T>* root) {
  switch(order) {
    case Order::Pre:
      std::swap(funcs_[0], funcs_[1]);
    break;
    case Order::Post:
      std::swap(funcs_[1], funcs_[2]);
    break;
  }
  return VisitImpl(root);
}

template <typename T>
void BSTVisitor<T>::VisitImpl(TreeNode<T>* root) {
  if (root != nullptr) {
    funcs_[0](root);
    funcs_[1](root);
    funcs_[2](root);
  }
}
