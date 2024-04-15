#include<stdio.h>
#include<stddef.h>
#include<stdlib.h>

#define SIZE 10000

char localheap[SIZE];

typedef struct Meta_data
{
    size_t size;
    int flag;                            //flag 0 if free else 1
    struct Meta_data* next;
}meta_data;

typedef struct segment
{
    int value;
    int index;
    int no_of_ptr;
    int flag1;
    struct segment* next[3];
}segment;

meta_data* free_space=(void*)localheap;

void  divide_space(meta_data* , size_t );

void merge_function();

void initialize_heap() 
{
   
    free_space->size = SIZE - sizeof(meta_data);
    free_space->flag = 1;
    free_space->next = NULL;
}

void divide_space(meta_data *ptr, size_t size) 
{
    meta_data *new_block = (void *)(((void *)ptr) + size + sizeof(meta_data));
    new_block->next = ptr->next;
    new_block->size = ptr->size - size - sizeof(meta_data);
    new_block->flag = 1;
    ptr->flag=0;
    ptr->next = new_block;
    ptr->size = size;
   
}

void* allocate_memory(size_t size) 
{
    meta_data *ptr = free_space, *best_fit = NULL;
    void* return_ptr;
    int flag=0;

    while (ptr != NULL) 
    {
        if (ptr->flag==1 && ptr->size >= size && (best_fit == NULL || ptr->size < best_fit->size)) 
        {
            best_fit = ptr;
        }
        ptr = ptr->next;
    }

    if (best_fit != NULL)
     {
      
      
       if((best_fit->size)>size+sizeof(meta_data))
        {
            divide_space(best_fit, size);
           flag=1;

        }
        
        else
        {   
            if(best_fit->size == size) 
            {
              best_fit->flag = 0;
             flag=1;
            }
        }


        return_ptr = (void *)(best_fit + 1); // Adjusting the pointer to skip metadata
        
       
    } 

    else 
    {
        printf("no sufficient storage to allocate\n");
        return_ptr = NULL;
    }

    if(flag==1)
{
    printf("memory has been allocated\n");
}

    return return_ptr;
}

void free_storage(void *ptr)
 {
    if (((void *)free_space <= ptr) && (ptr <= (void *)(free_space + SIZE))) 
    {
        segment* sptr;
        sptr=ptr;
        printf("the segment value : %d \n",sptr->value);
        meta_data *ptr1 = ptr;
        --ptr1;
        ptr1->flag = 1;
        merge_function();  
        printf("Freed memory size: %d bytes\n", ptr1->size);
    } 

    else 
    {
        printf("invalid pointer not in range of heap \n\n");
    }
    
}

void merge_function()
 {
    meta_data *ptr = free_space;
    while (ptr != NULL && ptr->next != NULL) 
    {
        if (ptr->flag==1 && ptr->next->flag==1)
         {
            ptr->size = ptr->size + ptr->next->size + sizeof(meta_data);
            ptr->next = ptr->next->next;
            printf("\nmerged memory size: %d  bytes\n",ptr->size);
         }   

         else
         {    
          ptr = ptr->next;
         }
    }

   
}



void initailize_adjacancy_matrix(int adj_matrix[8][8])
{
    int i,j;
    for (i = 0; i < 8; i++)
    { 
       for (j = 0; j < 8; j++)
       {
         adj_matrix[i][j] = 0;
       }
    }
}


void adj_matrix(segment* root,int adj_mat[8][8])
{
    int i;
     for (i = 0; i < root->no_of_ptr; i++)
    {
        adj_mat[root->index][root->next[i]->index] = 1;
        adj_matrix(root->next[i],adj_mat);
    }
}


void final_adj_matrix(int final[8][8],int adj1[8][8],int adj2[8][8])
{
    int i,j;
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            
            if (adj1[i][j] == 1 || adj2[i][j] == 1)
            {
                final[i][j] = 1;
            }
            else
            {
                final[i][j] = 0;
            }
        }
    }
}


void ref_count(segment* arr[8],int adj1[8][8],int adj2[8][8])
{
    int f[8][8];
    final_adj_matrix(f,adj1,adj2);
    int no_of_incoming_ptrs;
    int i,j;
    for (j= 0;j< 8; j++)
    {
     if (arr[j]->flag1== 0)
    {
        no_of_incoming_ptrs = 0;
        for (i= 0; i< 8; i++)
        {
           if (f[i][j] == 1)
            {
             no_of_incoming_ptrs++;
            }
        }
         if (no_of_incoming_ptrs==0)
        {
           free_storage((void *)arr[j]);
        }
    }
    }
}

segment* newsegment(int value,int index,int n_ptr)
{
     segment* nptr=(segment*)(allocate_memory(sizeof(segment)));
     nptr->value=value;
     nptr->index=index;
     nptr->flag1=0;
     nptr->no_of_ptr=n_ptr;
     return nptr;
}


void sweep(segment *arr[8])
{
    int i;
    for (i = 0; i < 8; i++)
    {
        if (arr[i]->flag1 == 0)
        {
            free_storage((void *)arr[i]);
        }

        if (i != 0 && i != 3)
        {
            arr[i]->flag1 = 0;
        }
    }
}

void mark(segment* arr[8],int adj1[8][8],int adj2[8][8])
{
   int no_of_incoming_ptrs;
   int final[8][8];
   final_adj_matrix(final,adj1,adj2);
   int i,j;
   for(j=0;j<8;j++)
   {
    if(arr[j]->flag1==0)
    {
    no_of_incoming_ptrs =0;
    for(i=0;i<7;i++)
    {
        if(final[i][j]==0)
        {
            no_of_incoming_ptrs++;
        }
    }

    if(no_of_incoming_ptrs!=0)
    {
        arr[j]->flag1=1;

    }
    }
   
   else
   {
    arr[j]->flag1=1;
   }
   }
}

void print_aj_mat(int adj[8][8])
{
    int i,j;
    for(i=0;i<8;i++)
    {
        for(j=0;j<8;j++)
        {
            printf("%d ",adj[i][j]);
        }
        printf("\n");
    }
}

typedef struct ROOT
{
    segment* root;
    struct ROOT* next;
}root1;


int main()
{
    initialize_heap();
    int adj_mat1[8][8];
    int adj_mat2[8][8];
    int final_adj_mat[8][8];
    initailize_adjacancy_matrix(adj_mat1);
    initailize_adjacancy_matrix(adj_mat2);
     
    segment* s1=newsegment(1,0,3);
    segment* s2=newsegment(2,1,0);
    segment* s3=newsegment(3,2,2);
    segment* s5=newsegment(5,3,1);
    segment* s7=newsegment(7,4,2);
    segment* s8=newsegment(8,5,1);
    segment* s9=newsegment(9,6,0);
    segment* s10=newsegment(10,7,0);

    s1->next[0] = s2;
    s1->next[1] = s9;
    s1->next[2] = s10;
    s3->next[0] = s8;
    s3->next[1] = s10;
    s5->next[0] = s1;
    s7->next[0] = s1;
    s7->next[1] = s8;
    s8->next[0] = s9;
   
    root1 root_a,root_b;
    root_a.root=s5;
    s5->flag1=1;
    root_b.root=s1;
    s1->flag1=1;
    root_a.next=&root_b;
    root_b.next=NULL;


    adj_matrix(root_a.root,adj_mat1);
    adj_matrix(root_b.root,adj_mat2);

    printf("\nroot_a adj matrix\n");
    print_aj_mat(adj_mat1);
    printf("\n\n");
    printf("\nroot_b adj matrix\n");
    print_aj_mat(adj_mat2);


    segment* arr[8]={s1,s2,s3,s5,s7,s8,s9,s10};


    printf("\nreference count garbage collection : \n\n");

    ref_count(arr,adj_mat1,adj_mat2);

    printf("\nroot_a adj matrix\n");
    print_aj_mat(adj_mat1);
    printf("\n\n");
    printf("\nroot_b adj matrix\n");
    print_aj_mat(adj_mat2);
   

    printf("\n mark sweep garbage collection : \n\n");
    mark(arr,adj_mat1,adj_mat2);
    sweep(arr);
    printf("\nroot_a adj matrix\n");
    print_aj_mat(adj_mat1);
    printf("\n\n");
    printf("\nroot_b adj matrix\n");
    print_aj_mat(adj_mat2);

      
}