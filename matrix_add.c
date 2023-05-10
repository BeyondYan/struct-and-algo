#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct cross_matrix_node{
    int r,c;
    int val;
    struct cross_matrix_node*right;
    struct cross_matrix_node*down;
}cmn,*m_n;

typedef struct cross_matrix{
     m_n* rank_listhead;//可申请node数组
     m_n* column_listhead;//指针可申请列头结点储存所在的一维节点
     int nrow;
     int ncol;
}CM;//CM就能包含一个数组的全部信息，到时候数组相加就是CM相加

void init_CM(CM *cml,int r,int c){//cml是十字链表类变量名称
    int i,j;
    cml->column_listhead=(m_n*)malloc(sizeof(m_n)*c);
    cml->rank_listhead=(m_n*)malloc(sizeof(m_n)*r);
    for(i=0;i<r;i++){
        cml->rank_listhead[i]=NULL;
    }
    for(j=0;j<c;j++){
        cml->column_listhead[j]=NULL;
    }
    cml->nrow=r;
    cml->ncol=c;
}

CM* arrytomatrix(int*pbase,int r,int c,int t){//二维数组基址，行数，列数，非零元 个数
    CM* ocm = (CM*)malloc(sizeof(CM));
    init_CM (ocm,r,c);
    m_n pnew,qfront;//用于生成三元组节点
    int i,j;
    for(i=0;i<r;i++){
        for(j=0;j<c;j++){
            if(*(pbase+i*c+j)!=0){
                pnew=(m_n)malloc(sizeof(cmn));
                pnew->r=i;
                pnew->c=j;
                pnew->val=*(pbase+i*c+j);
                pnew->right=NULL;
                pnew->down=NULL;
                if(ocm->rank_listhead[i]==NULL){
                    ocm->rank_listhead[i]=pnew;
                }
                else{
                    for(qfront=ocm->rank_listhead[i];qfront->right!=NULL;qfront=qfront->right);
                    qfront->right=pnew;
                }

                if(ocm->column_listhead[j]==NULL)
                    ocm->column_listhead[j]=pnew;
                else{
                    for(qfront=ocm->column_listhead[j];qfront->down!=NULL;qfront=qfront->down);
                    qfront->down=pnew;
                }
            }
        }
    }



    return ocm;
}

void display_crossMartix(CM* matrix){
    int re0=0;
    m_n prank;
    for(int i=0;i<matrix->nrow;i++){
        re0=-1;
        if(matrix->rank_listhead[i]!=NULL){
        	int dis_bond=0;
            prank=matrix->rank_listhead[i];
            while(prank){
            	int cnt1=prank->c-re0-1;
            	
                while(cnt1){
                    printf("%d   ",0);
                    cnt1--;
                }
                re0=prank->c;
                printf("%d   ",prank->val);
                prank=prank->right;
            }
            dis_bond=matrix->ncol-re0-1;
            while(dis_bond){//此处是因为矩阵满秩列的下标由于数组的填入方式，比列数小1
                printf("%d   ",0);
                dis_bond--;
            }
        }
        else{
            for(int j=0;j<matrix->ncol;j++){
                printf("0   ");
            }
        }
        printf("\n");
//        if(i==matrix->nrow-1)
//        	break;
    }
}

//popurse:to use 十字链表 achieve the add_operation
m_n addofList(m_n list1,m_n list2){
    m_n l1,l2,pm;
    l1=list1;
    l2=list2;
    m_n Pret=(m_n)malloc(sizeof(cmn));
    Pret->down=NULL;
    Pret->right=NULL;
    pm=Pret;
    if(l1==NULL&&l2!=NULL)
        return l2;
    else if(l2==NULL&&l1!=NULL)
        return l1;
    else if(l1==NULL && l2==NULL)
        return NULL;
    else{
        while(l1 && l2){
            if(l1->c < l2->c){
                pm->right=l1;
                pm=pm->right;
                l1=l1->right;
            }//pm->right->val 
            else if(l1->c > l2->c){
                pm->right=l2;
                pm=pm->right;
                l2=l2->right;
            }
            else{
                l1->val=l1->val+l2->val;
                pm->right=l1;
                pm=pm->right;
                l1=l1->right;
                l2=l2->right;   //********倒数第二次修改：2链值加到一链上后还应将2链当前位置后移 
            } 

        }
        if(l1)
                pm->right=l1;
            else
                pm->right=l2;
    }//这里仅用数值进行比较是毫无意义的，没有将同位置数值相加，而且超出了访问的边界 
    return Pret->right;

}
CM* add_matrix(CM* m1,CM* m2,int row,int col){//两个矩阵相加，十字链表对应行列的链表相加，实际就是多个链表相加
    CM* ret_matrix=NULL;
    ret_matrix = (CM*)malloc(sizeof(CM));
    init_CM(ret_matrix,row,col);
    for(int i=0;i<row;i++){
        ret_matrix->rank_listhead[i]=addofList(m1->rank_listhead[i],m2->rank_listhead[i]);
    }
//    for(int j=0;j<col;j++){
//        ret_matrix->column_listhead[j]=addofList(m1->column_listhead[j],m2->column_listhead[j]);
//    }//最后一处修改，因为只用add没有关于列的加和操作 
    return ret_matrix;
}


int main(){
    int base[4][4]={{0,2,0,0,},
                    {1,0,3,0,},
                    {0,0,0,4,},
                    {0,0,5,6,},
    };
    CM* manip=NULL;
    manip=arrytomatrix((int*)base,4,4,6);
    display_crossMartix(manip);
    printf("\n");
    int arry[4][4]={{3,0,1,0,},
                    {0,0,0,1,},
                    {1,0,0,0,},
                    {0,0,2,0,},

    };
    CM* plusm=NULL;
    plusm=arrytomatrix((int*)arry,4,4,5);
    display_crossMartix(plusm);
    printf("\n");
    CM* addresult=NULL;
    addresult=add_matrix(manip,plusm,4,4);
    display_crossMartix(addresult);
}