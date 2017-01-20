#ifndef BINNODE_H
#define BINNODE_H

#include <stack>
#include <queue>
#include <cmath>

using namespace std;

#define BinNodePosi(T) BinNode<T>* //节点位置
#define stature(p) ((p) ? (p)->height : -1) //节点高度（与“空树高度为-1”的约定相统一）
/******************************************************************************************
 * BinNode状态与性质的判断
 ******************************************************************************************/
#define IsRoot(x) ( ! ( (x).parent ) )
#define IsLChild(x) ( ! IsRoot(x) && ( & (x) == (x).parent->lc ) )
#define IsRChild(x) ( ! IsRoot(x) && ( & (x) == (x).parent->rc ) )
#define HasParent(x) ( ! IsRoot(x) )
#define HasLChild(x) ( (x).lc )
#define HasRChild(x) ( (x).rc )
#define HasChild(x) ( HasLChild(x) || HasRChild(x) ) //至少拥有一个孩子
#define HasBothChild(x) ( HasLChild(x) && HasRChild(x) ) //同时拥有两个孩子
#define IsLeaf(x) ( ! HasChild(x) )

/******************************************************************************************
 * 与BinNode具有特定关系的节点及指针
 ******************************************************************************************/
/*#define sibling(p) //兄弟 \
   ( IsLChild( * (p) ) ? (p)->parent->rc : (p)->parent->lc )*/

#define uncle(x) /*叔叔*/ \
   ( IsLChild( * ( (x)->parent ) ) ? (x)->parent->parent->rc : (x)->parent->parent->lc )

/*#define FromParentTo(x)  \
   ( IsRoot(x) ? this->_root : ( IsLChild(x) ? (x).parent->lc : (x).parent->rc ) )*/

#define HeightUpdated(x) /*高度更新常规条件*/ \
       ( (x).height == 1 + max( stature( (x).lc ), stature( (x).rc ) ) )
#define Balanced(x) ( stature( (x).lc ) == stature( (x).rc ) ) //理想平衡条件
#define BalFac(x) ( stature( (x).lc ) - stature( (x).rc ) ) //平衡因子
#define AvlBalanced(x) ( ( -2 < BalFac(x) ) && ( BalFac(x) < 2 ) ) //AVL平衡条件

#define IsBlack(p) ( ! (p) || ( RB_BLACK == (p)->color ) ) //外部节点也视作黑节点
#define IsRed(p) ( ! IsBlack(p) ) //非黑即红
#define BlackHeightUpdated(x) ( /*RedBlack高度更新条件*/ \
   ( stature( (x).lc ) == stature( (x).rc ) ) && \
   ( (x).height == ( IsRed(& x) ? stature( (x).lc ) : stature( (x).lc ) + 1 ) ) \
)

typedef enum { RB_RED, RB_BLACK} RBColor; //节点颜色

template <typename T> struct BinNode { //二叉树节点模板类
// 成员（为简化描述起见统一开放，读者可根据需要进一步封装）
   T data; //数值
   BinNodePosi(T) parent; BinNodePosi(T) lc; BinNodePosi(T) rc; //父节点及左、右孩子
   int height; //高度（通用）
   int npl; //Null Path Length（左式堆，也可直接用height代替）
   RBColor color; //颜色（红黑树）
// 构造函数
   BinNode() :
      parent ( NULL ), lc ( NULL ), rc ( NULL ), height ( 0 ), npl ( 1 ), color ( RB_RED ), level(0), x(0), y(0) { }
   BinNode ( T e, BinNodePosi(T) p = NULL, BinNodePosi(T) lc = NULL, BinNodePosi(T) rc = NULL,
             int h = 0, int l = 1, RBColor c = RB_RED ) :
      data ( e ), parent ( p ), lc ( lc ), rc ( rc ), height ( h ), npl ( l ), color ( c ), level(0), x(0), y(0) { }
// 操作接口
   int size(); //统计当前节点后代总数，亦即以其为根的子树的规模
   BinNodePosi(T) insertAsLC ( T const& ); //作为当前节点的左孩子插入新节点
   BinNodePosi(T) insertAsRC ( T const& ); //作为当前节点的右孩子插入新节点
   BinNodePosi(T) succ(); //取当前节点的直接后继
   template <typename VST> void travLevel ( VST& ); //子树层次遍历
   template <typename VST> void travPre ( VST& ); //子树先序遍历
   template <typename VST> void travIn ( VST& ); //子树中序遍历
   template <typename VST> void travPost ( VST& ); //子树后序遍历
// 比较器、判等器
   bool operator< ( BinNode const& bn ) { return data < bn.data; } //小于
   bool operator== ( BinNode const& bn ) { return data == bn.data; } //等于
   BinNodePosi(T) zig(); //顺时针旋转
   BinNodePosi(T) zag(); //逆时针旋转
   //可视化参数
   int level;//层数
   int updateLevel();//更新层数, 返回最大层数
   int x;//横坐标
   int y;//纵坐标
   int updatePosition(int levelMargin, int sibMargin, int margin, int r, int leftMargin);//更新坐标
};

template <typename T> int BinNode<T>::size() { //统计当前节点后代总数，即以其为根的子树规模
   int s = 1; //计入本身
   if ( lc ) s += lc->size(); //递归计入左子树规模
   if ( rc ) s += rc->size(); //递归计入右子树规模
   return s;
}

template <typename T> BinNodePosi(T) BinNode<T>::insertAsLC ( T const& e )
{ return lc = new BinNode ( e, this ); } //将e作为当前节点的左孩子插入二叉树

template <typename T> BinNodePosi(T) BinNode<T>::insertAsRC ( T const& e )
{ return rc = new BinNode ( e, this ); } //将e作为当前节点的右孩子插入二叉树

template <typename T> BinNodePosi(T) BinNode<T>::succ() { //定位节点v的直接后继
   BinNodePosi(T) s = this; //记录后继的临时变量
   if ( rc ) { //若有右孩子，则直接后继必在右子树中，具体地就是
      s = rc; //右子树中
      while ( HasLChild ( *s ) ) s = s->lc; //最靠左（最小）的节点
   } else { //否则，直接后继应是“将当前节点包含于其左子树中的最低祖先”，具体地就是
      while ( IsRChild ( *s ) ) s = s->parent; //逆向地沿右向分支，不断朝左上方移动
      s = s->parent; //最后再朝右上方移动一步，即抵达直接后继（如果存在）
   }
   return s;
}

//从当前节点出发，沿左分支不断深入，直至没有左分支的节点；沿途节点遇到后立即访问
template <typename T, typename VST> //元素类型、操作器
static void visitAlongLeftBranch ( BinNodePosi(T) x, VST& visit, stack<BinNodePosi(T)>& S ) {
   while ( x ) {
      visit ( x->data ); //访问当前节点
      S.push ( x->rc ); //右孩子入栈暂存（可优化：通过判断，避免空的右孩子入栈）
      x = x->lc;  //沿左分支深入一层
   }
}

template <typename T, typename VST> //元素类型、操作器
void travPre_I2 ( BinNodePosi(T) x, VST& visit ) { //二叉树先序遍历算法（迭代版#2）
   stack<BinNodePosi(T)> S; //辅助栈
   while ( true ) {
      visitAlongLeftBranch ( x, visit, S ); //从当前节点出发，逐批访问
      if ( S.empty() ) break; //直到栈空
      {
          x = S.top();
          S.pop();
      }//弹出下一批的起点
   }
}

template <typename T> template <typename VST> //元素类型、操作器
void BinNode<T>::travPre ( VST& visit ) { //二叉树先序遍历算法统一入口
    travPre_I2 ( this, visit );
}

template <typename T> //从当前节点出发，沿左分支不断深入，直至没有左分支的节点
static void goAlongLeftBranch ( BinNodePosi(T) x, stack<BinNodePosi(T)>& S ) {
   while ( x ) { S.push ( x ); x = x->lc; } //当前节点入栈后随即向左侧分支深入，迭代直到无左孩子
}

template <typename T, typename VST> //元素类型、操作器
void travIn_I1 ( BinNodePosi(T) x, VST& visit ) { //二叉树中序遍历算法（迭代版#1）
   stack<BinNodePosi(T)> S; //辅助栈
   while ( true ) {
      goAlongLeftBranch ( x, S ); //从当前节点出发，逐批入栈
      if ( S.empty() ) break; //直至所有节点处理完毕
      x = S.top(); S.pop(); visit ( *x ); //弹出栈顶节点并访问之
      x = x->rc; //转向右子树
   }
}

template <typename T> template <typename VST> //元素类型、操作器
void BinNode<T>::travIn ( VST& visit ) { //二叉树中序遍历算法统一入口
    travIn_I1 ( this, visit );
}

template <typename T> //在以S栈顶节点为根的子树中，找到最高左侧可见叶节点
static void gotoHLVFL ( stack<BinNodePosi(T)>& S ) { //沿途所遇节点依次入栈
   while ( BinNodePosi(T) x = S.top() ) //自顶而下，反复检查当前节点（即栈顶）
      if ( HasLChild ( *x ) ) { //尽可能向左
         if ( HasRChild ( *x ) ) S.push ( x->rc ); //若有右孩子，优先入栈
         S.push ( x->lc ); //然后才转至左孩子
      } else //实不得已
         S.push ( x->rc ); //才向右
   S.pop(); //返回之前，弹出栈顶的空节点
}

template <typename T, typename VST>
void travPost_I ( BinNodePosi(T) x, VST& visit ) { //二叉树的后序遍历（迭代版）
   stack<BinNodePosi(T)> S; //辅助栈
   if ( x ) S.push ( x ); //根节点入栈
   while ( !S.empty() ) {
      if ( S.top() != x->parent ) //若栈顶非当前节点之父（则必为其右兄），此时需
         gotoHLVFL ( S ); //在以其右兄为根之子树中，找到HLVFL（相当于递归深入其中）
      x = S.top(); S.pop(); visit ( *x ); //弹出栈顶（即前一节点之后继），并访问之
   }
}

template <typename T> template <typename VST> //元素类型、操作器
void BinNode<T>::travPost ( VST& visit ) { //二叉树后序遍历算法统一入口
    travPost_I ( this, visit );
}

template <typename T>
int BinNode<T>::updateLevel() {
    int maxLevel = 0;
    queue<BinNodePosi(T)> Q; //辅助队列
    Q.push(this);//根节点入队
    while ( !Q.empty() ) { //在队列再次变空之前，反复迭代
       BinNodePosi(T) x = Q.front();
       Q.pop();
       maxLevel = x->level = IsRoot( *x ) ? 0 : x->parent->level + 1;
       if ( HasLChild ( *x ) ) Q.push ( x->lc ); //左孩子入队
       if ( HasRChild ( *x ) ) Q.push ( x->rc ); //右孩子入队
    }
    return maxLevel;
}

template <typename T>
int BinNode<T>::updatePosition(int levelMargin, int sibMargin, int margin, int r, int leftMargin) {
    int maxLevel = this->updateLevel();//更新层数，记录最大层数
    int maxWidth = 0;
    if(maxLevel > 0)
    {
        int sibMargin_n = sibMargin - (margin * (pow(2, maxLevel - 1) - 1)) / pow(2, maxLevel - 1);
        maxWidth = pow(2, maxLevel - 1) * sibMargin_n + 2 * r * pow(2, maxLevel) +
                (pow(2, maxLevel - 1) - 1) * margin;
        maxWidth -= leftMargin;
    }
    else
        maxWidth = 2 * r;
    int rootX = maxWidth / 2 - r;
    int rootY = r;
    queue<BinNodePosi(T)> Q; //辅助队列
    Q.push(this);//根节点入队
    while ( !Q.empty() ) { //在队列再次变空之前，反复迭代
       BinNodePosi(T) x = Q.front();
       Q.pop();
       //update x
       if(IsRoot(*x))
           x->x = rootX;
       else
       {
           int sibMargin_n = sibMargin - (margin * (pow(2, x->level - 1) - 1)) / pow(2, x->level - 1);
           x->x = x->parent->x + ((IsLChild( *x )) ? (0 - sibMargin_n / 2) : (sibMargin_n / 2));
       }
       //update y
       if(IsRoot(*x))
           x->y = rootY;
       else
           x->y = x->parent->y + levelMargin + 2 * r;
       if ( HasLChild ( *x ) ) Q.push ( x->lc ); //左孩子入队
       if ( HasRChild ( *x ) ) Q.push ( x->rc ); //右孩子入队
    }
    return maxLevel;
}

template <typename T> template <typename VST> //元素类型、操作器
void BinNode<T>::travLevel ( VST& visit ) { //二叉树层次遍历算法
   queue<BinNodePosi(T)> Q; //辅助队列
   Q.push(this);//根节点入队
   while ( !Q.empty() ) { //在队列再次变空之前，反复迭代
      BinNodePosi(T) x = Q.front();
      Q.pop();
      //visit ( x->data ); //取出队首节点并访问之
      visit ( *x ); //可视化访问接口
      if ( HasLChild ( *x ) ) Q.push ( x->lc ); //左孩子入队
      if ( HasRChild ( *x ) ) Q.push ( x->rc ); //右孩子入队
   }
}

template <typename T> BinNodePosi(T) BinNode<T>::zig() { //顺时针旋转
   BinNodePosi(T) lChild = lc;
   lChild->parent = this->parent;
   if ( lChild->parent )
      ( ( this == lChild->parent->rc ) ? lChild->parent->rc : lChild->parent->lc ) = lChild;
   lc = lChild->rc; if ( lc ) lc->parent = this;
   lChild->rc = this; this->parent = lChild;
   return lChild;
}

template <typename T> BinNodePosi(T) BinNode<T>::zag() { //逆时针旋转
   BinNodePosi(T) rChild = rc;
   rChild->parent = this->parent;
   if ( rChild->parent )
      ( ( this == rChild->parent->lc ) ? rChild->parent->lc : rChild->parent->rc ) = rChild;
   rc = rChild->lc; if ( rc ) rc->parent = this;
   rChild->lc = this; this->parent = rChild;
   return rChild;
}

//通过zig旋转调整，将子树x拉伸成最右侧通路
template <typename T> void stretchByZig ( BinNodePosi(T) & x, int h ) {
   for ( BinNodePosi(T) v = x; v; v = v->rc ) {
      while ( v->lc )
         v = v->zig();
      v->height = --h;
   }
   while ( x->parent ) x = x->parent;
}

//通过zag旋转调整，将子树x拉伸成最左侧通路
template <typename T> void stretchByZag ( BinNodePosi(T) & x ) {
   int h = 0;
   BinNodePosi(T) p = x; while ( p->rc ) p = p->rc; //最大节点，必是子树最终的根
   while ( x->lc ) x = x->lc; x->height = h++; //转至初始最左侧通路的末端
   for ( ; x != p; x = x->parent, x->height = h++ ) { //若x右子树已空，则上升一层
      while ( x->rc ) //否则，反复地
         x->zag(); //以x为轴做zag旋转
   } //直到抵达子树的根
}

#endif // BINNODE_H
