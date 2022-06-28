#include <iostream>
#include <cstring>
#include <cstdio>
using namespace std;


typedef char *line;
typedef struct text {int max_length;
                int line_number;
                struct text *left;
                struct text *right;
                int height;
                } text_t;


#define BLOCKSIZE 256


text_t *currentblock = NULL;
text_t *free_list = NULL;
int size_left;
int nodes_taken = 0;
int nodes_returned = 0;

void insert_line(text_t *txt, int index, char *new_line);

int string_length(char* input_string)
{
    // variable to store the
    // length of the string
    int length = 0;
    while (*input_string != '\0') {
        length++;
        input_string++;
    }
    return length;
}
int length_text(text_t *txt)
{
    if(txt->left==NULL)
    {
        return 0;
    }
    if(txt->right==NULL)
    {
        return 1;
    }
    else
    {
        return length_text(txt->left)+length_text(txt->right);
    }
}


int get_line_size(text_t *txt, int index)
{
    if (length_text(txt) < index)
        return -1;
    text_t *tmp_node = txt;
    while( tmp_node->right != NULL )
    {   if( index <= tmp_node->line_number )
            tmp_node = tmp_node->left;
        else
        {
            index = index - tmp_node->line_number;
            tmp_node = tmp_node->right;
        }
    }
    return( string_length((char *) tmp_node->left));
}


int set_max_length(text_t *txt, int max_length)
{
    txt->max_length = max_length;
    return txt->max_length;
}

void return_node(text_t *node)
{  node->right = free_list;
   free_list = node;
   nodes_returned +=1;
}

text_t *get_node()
{ text_t *tmp;
  nodes_taken += 1;
  if( free_list != NULL )
  {  tmp = free_list;
     free_list = free_list -> right;
  }
  else
  {  if( currentblock == NULL || size_left == 0)
     {  currentblock =
                (text_t *) malloc( BLOCKSIZE * sizeof(text_t) );
        size_left = BLOCKSIZE;
     }
     tmp = currentblock++;
     size_left -= 1;
  }
  return( tmp );
}


text_t *create_tree(void)
{  text_t *tmp_node;
   tmp_node = get_node();
   tmp_node->left = NULL;
   return( tmp_node );
}


int count_leaf(text_t *node)
{
  if(node == NULL)      
    return 0;
  if(node->right==NULL)     
    return 1;           
  else
    return node->line_number+count_leaf(node->right);     
}


void right_rotation(text_t *n)
{
    text_t *tmp_node;
    int tmp_line_number;
    tmp_node = n->right;
    tmp_line_number = n->line_number;
    n->right = n->left;
    n->line_number = n->left->line_number;
    n->left = n->right->left;
    n->right->left = n->right->right;
    n->right->right = tmp_node;
    //n->right->line_number = tmp_line_number;
    n->right->line_number = count_leaf(n->right->left);
    //printf("Line number in right_rotation: %d\n", tmp_line_number);
    cout << "\nLine number in right_rotation:\t" << tmp_line_number;
}


void left_rotation(text_t *n)
{
    text_t *tmp_node;
    int tmp_line_number;
    tmp_node = n->left;
    tmp_line_number = n->line_number;
    n->left = n->right;
    n->right = n->left->right;
    n->left->right = n->left->left;
    n->left->left = tmp_node;
    n->left->line_number = tmp_line_number;
    n->line_number = count_leaf(n->left);
    cout << "\nLine number in left_rotation:\t" << tmp_line_number;
}

void update_nodes(text_t *txt, int index, char operation)
{
    int number;
    if(operation=='i')
    {
        number = 1;
    }
    else
    {
        number = -1;
    }
    
    if (txt==NULL)
    {
        return;
    }
    if (txt->left==NULL)
    {
        return;
    }
    if (txt->line_number>=index)       // in case of deletion it should be > line_number
    {
        txt->line_number+=number;
        cout << "\nupdated line number::\t" << txt->line_number;
        
    }
    update_nodes(txt->left, index, operation);
    update_nodes(txt->right, index, operation);
}

void update_nodes_delete(text_t *txt, int index, char operation)
{
    int number=-1;
    
    if (txt==NULL)
    {
        return;
    }
    if (txt->left==NULL)
    {
        return;
    }
    if (txt->line_number>index)       // in case of deletion it should be > line_number
    {
        txt->line_number+=number;
        cout << "\nupdated line number::\t" << txt->line_number;
        
    }
    update_nodes_delete(txt->left, index, operation);
    update_nodes_delete(txt->right, index, operation);
}



text_t *create_text(void)
{  text_t *tmp_node;
   tmp_node = get_node();
   tmp_node->left = NULL;
   return( tmp_node );
}




char *get_line(text_t *txt, int index)
{  text_t *tmp_node;
   if( txt->left == NULL )
     return(NULL);
   else
   {  tmp_node = txt;
      while( tmp_node->right != NULL )
      {   if( index <= tmp_node->line_number )
               tmp_node = tmp_node->left;
          else
          {
              index = index - tmp_node->line_number;
              tmp_node = tmp_node->right;
          }
      }
      if( tmp_node->line_number == index )
      {
          return( (char *) tmp_node->left );
      }
         
      else
      {
        return( NULL );
      }
         
   }
}


void append_line(text_t *txt, char *new_line)
{
    text_t *tmp_node;
    if(strlen(new_line)>txt->max_length)
    {
        cout << "Failed";
    }
    else
    {
       tmp_node = txt;
       insert_line(txt, length_text(tmp_node)+1, new_line);
       
    }
}


text_t *find_iterative(text_t *txt, int query_key)
{  text_t *tmp_node;
   if( txt->left == NULL )
     return(NULL);
   else
   {  tmp_node = txt;
      while( tmp_node->right != NULL )
      {   if( query_key <= tmp_node->line_number )
               tmp_node = tmp_node->left;
          else
          {
              query_key = query_key - tmp_node->line_number;
              tmp_node = tmp_node->right;
          }
               
      }
      //printf("%d", query_key);
      if( tmp_node->line_number == query_key )
         return(tmp_node);
      else
         return( NULL );
   }
}

char *set_line( text_t *txt, int index, char * new_line){
    text_t * tmp_node = find_iterative(txt, index);
    cout << "\nfound line number: \t" << tmp_node->line_number;
    
    if(tmp_node==NULL){
        return NULL;
    }
    else{
        char *previous_line = (char *) tmp_node->left;
        tmp_node->left = (text_t *) new_line;
        cout << "\nset line: \t" << (char *)tmp_node->left;
        return previous_line;
    }
}



void insert_line(text_t *txt, int index, char *new_line)
{  text_t *tmp_node;
   int actual_number = index; 
    if(strlen(new_line)>txt->max_length)
    {
        return NULL;
    }
   if( txt->left == NULL )
   {  txt->left = (text_t *) new_line;
      txt->line_number  = 1;//change here
       txt->height = 0;
      txt->right  = NULL;
   }
   else
   {
       //update_nodes(txt, index, 'i');
       text_t *stack[200]; int stack_p = 0;
       tmp_node = txt;
      while( tmp_node->right != NULL )
      {
          stack[stack_p++] = tmp_node;
          // printf("pushed items: %d\n", tmp_node->line_number);
          cout << "\npushed items: \t" << tmp_node->line_number;
          if( index <= tmp_node->line_number )
               tmp_node = tmp_node->left;
          else
          {
              index = index - tmp_node->line_number;
              tmp_node = tmp_node->right;                 
          }
      }
      /* found the candidate leaf. Test whether line_number distinct */
      //if( tmp_node->line_number == index)
      // return( -1 );
      /* line_number is distinct, now perform the insert */
      text_t *old_leaf, *new_leaf;
     old_leaf = get_node();
     old_leaf->left = tmp_node->left;
     old_leaf->line_number = tmp_node->line_number;
     old_leaf->right  = NULL;
     old_leaf->height = 0;
     new_leaf = get_node();
     new_leaf->left = (text_t *) new_line;
     new_leaf->line_number = 1;
     new_leaf->right  = NULL;
     new_leaf->height = 0;
     if( tmp_node->line_number < index)//actual_number )
     {   tmp_node->left  = old_leaf;
         tmp_node->right = new_leaf;
         tmp_node->line_number = old_leaf->line_number;//1
     }
     else
     {   tmp_node->left  = new_leaf;
         tmp_node->right = old_leaf;
         tmp_node->line_number = new_leaf->line_number;//1
     }
      tmp_node->height = 1;
      
       
       int finished = 0;
       
       while(stack_p>0 && !finished)
       {
           int tmp_height, old_height;
           tmp_node = stack[--stack_p];
           old_height = tmp_node->heig  ht;

           if(tmp_node->left->height-tmp_node->right->height==2)
           {
               if(tmp_node->left->left->height-tmp_node->right->height==1)
               {
                   right_rotation(tmp_node);
                   tmp_node->right->height = tmp_node->right->left->height+1;
                   tmp_node->height = tmp_node->right->height+1;
               }
               else
               {
                   left_rotation(tmp_node->left);
                   right_rotation(tmp_node);
                   tmp_height = tmp_node->left->left->height;
                   tmp_node->left->height = tmp_height+1;
                   tmp_node->right->height = tmp_height+1;
                   tmp_node->height = tmp_height+2;
               }
           }
           else if(tmp_node->left->height-tmp_node->right->height==-2)
           {
               if(tmp_node->right->right->height-tmp_node->left->height==1)
               {
                   left_rotation(tmp_node);
                   tmp_node->left->height = tmp_node->left->right->height+1;
                   tmp_node->height = tmp_node->left->height+1;
               }
               else
               {
                   right_rotation(tmp_node->right);
                   left_rotation(tmp_node);
                   tmp_height = tmp_node->right->right->height;
                   tmp_node->left->height = tmp_height+1;
                   tmp_node->right->height = tmp_height+1;
                   tmp_node->height = tmp_height+2;
               }
           }
           else
           {
               if(tmp_node->left->height>tmp_node->right->height)
               {
                   tmp_node->height = tmp_node->left->height+1;
                   tmp_node->line_number = count_leaf(tmp_node->left);
               }
               else if(tmp_node->left->height==tmp_node->right->height)
               {
                   tmp_node->line_number = count_leaf(tmp_node->left);
               }
               else
               {
                   tmp_node->height = tmp_node->right->height+1;
               }
           }
           if(tmp_node->height==old_height)
           {
               finished=1;
           }
           
       }
       while(stack_p>0)
       {
           tmp_node = stack[--stack_p];
           tmp_node->line_number = count_leaf(tmp_node->left);
       }
       //remove stack or free stack memory
   }
}


char *delete_line(text_t *txt, int delete_line_number)
{  text_t *tmp_node, *upper_node, *other_node;
   char *deleted_object;
   
   if( txt->left == NULL )
      return( NULL );
   else if( txt->right == NULL )
   {  
       if(  txt->line_number == delete_line_number )
       {
           deleted_object = (char *) txt->left;
           txt->left = NULL;
           return( deleted_object );
       }
      else
         return( NULL );
   }
   else
   {  
        tmp_node = txt;
        text_t *stack_d[200]; int stack_p_d = 0;

        while( tmp_node->right != NULL )
        {   
            upper_node = tmp_node;
            if (upper_node->right->right!=NULL || (delete_line_number<=upper_node->line_number && upper_node->left->height>0))
            {
                stack_d[stack_p_d++] = upper_node;
            }
            if( delete_line_number <= tmp_node->line_number )
            {  
                tmp_node = upper_node->left;
                other_node = upper_node->right;
            }
            else
            {  
                delete_line_number = delete_line_number - tmp_node->line_number;
                tmp_node   = upper_node->right;
                other_node = upper_node->left;
            }
        }

        if( tmp_node->line_number != delete_line_number )
        {
            return( NULL );
        }
         
        
        //printf("line number%d\n", tmp_node->line_number);
        cout << "\nline number: \t" << tmp_node->line_number;
        //printf("outside loop: %d\n", upper_node->line_number);
        upper_node->line_number = other_node->line_number;
        upper_node->left  = other_node->left;
        upper_node->right = other_node->right;
        upper_node->height = other_node->height;
        deleted_object = (char *) tmp_node->left;
        // printf("outside loop: %s\n", deleted_object);
        cout << "\noutside loop: \t" << deleted_object;
        int finished_d = 0;
        while(stack_p_d>0 && finished_d==0)
        {
            int tmp_height, old_height;
            tmp_node = stack_d[--stack_p_d];
            // printf("popped items: %d\n", tmp_node->line_number);
            cout << "\npopped items: \t" << tmp_node->line_number;
            old_height = tmp_node->height;
            if(tmp_node->left->height-tmp_node->right->height==2)
            {
                if(tmp_node->left->left->height-tmp_node->right->height==1)
                {
                    right_rotation(tmp_node);
                    tmp_node->right->height = tmp_node->right->left->height+1;
                    tmp_node->height = tmp_node->right->height+1;
                }
                else
                {
                    left_rotation(tmp_node->left);
                    right_rotation(tmp_node);
                    tmp_height = tmp_node->left->left->height;
                    tmp_node->left->height = tmp_height+1;
                    tmp_node->right->height = tmp_height+1;
                    tmp_node->height = tmp_height+2;
                }
            }
            else if(tmp_node->left->height-tmp_node->right->height==-2)
            {
                if(tmp_node->right->right->height-tmp_node->left->height==1)
                {
                    tmp_node->line_number = count_leaf(tmp_node->left);
                    left_rotation(tmp_node);
                    tmp_node->left->height = tmp_node->left->right->height+1;
                    tmp_node->height = tmp_node->left->height+1;
                }
                else
                {
                    right_rotation(tmp_node->right);
                    left_rotation(tmp_node);
                    tmp_height = tmp_node->right->right->height;
                    tmp_node->left->height = tmp_height+1;
                    tmp_node->right->height = tmp_height+1;
                    tmp_node->height = tmp_height+2;
                }
            }
            else
            {
                if(tmp_node->left->height>tmp_node->right->height)
                {
                   tmp_node->height = tmp_node->left->height+1;
                }
                else
                {
                    tmp_node->height = tmp_node->right->height+1;
                }
                tmp_node->line_number = count_leaf(tmp_node->left);
            }
            if(tmp_node->height==old_height)
            {
                finished_d=1;
            }
            
        }
        while(stack_p_d>0)
        {
            tmp_node = stack_d[--stack_p_d];
            tmp_node->line_number = count_leaf(tmp_node->left);
        }
        //update_nodes_delete(txt, delete_line_number, 'd');
   }
   //returning the deleted object
   return( deleted_object );
}




void append_to_line(text_t *txt, char *new_substring)
{
    text_t *tmp_node;
    bool flag = false;
    if(txt->left ==  NULL)
    {
        flag = true;
        insert_line(txt, 1, new_substring);
    }
    else{
        text_t *stack[200]; int stack_p = 0;
        stack[stack_p++] = txt;
        
        while(stack_p>0)
        {
            tmp_node = stack[--stack_p];
            if(tmp_node->right==NULL)
            {
                char *old_string = (char *) tmp_node->left;
                char *old = old_string;
                if(string_length(old)+string_length(new_substring)<=txt->max_length)
                {
                    //concatenate the old string with new_substring
                    char *tmp = old;
                    while(*tmp)
                    {
                        tmp++;
                    }
                    while(*new_substring)
                    {
                        *tmp = *new_substring;
                        tmp++;
                        new_substring++;
                    }
                    *tmp = '\0';
                    
                    tmp_node->left = (text_t *)old;
                    // printf("Successfully Appended\n");
                    cout << "Successfully Appended\n";
                    flag = true;
                    break;
                }
            }
            else
            {
                if(tmp_node->right==NULL)
                {
                    break;
                }
                stack[stack_p++] = tmp_node->right;
                stack[stack_p++] = tmp_node->left;
                
            }
        }
        
    }
    if(!flag){
        append_line(txt, new_substring);
    }
    
    
    //remove stack or free stack memory
}


void remove_tree(text_t *tree)
{  text_t *current_node, *tmp;
   if( tree->left == NULL )
      return_node( tree );
   else
   {  current_node = tree;
      while(current_node->right != NULL )
      {  if( current_node->left->right == NULL )
         {  return_node( current_node->left );
            tmp = current_node->right;
            return_node( current_node );
            current_node = tmp;
         }
         else
         {  tmp = current_node->left;
            current_node->left = tmp->right;
            tmp->right = current_node;
            current_node = tmp;
         }
      }
      return_node( current_node );
   }
}

void inorder(text_t *txt)
{
    if(txt==NULL)
        return;
    else if(txt->right==NULL)
        return;
    inorder(txt->left);
    cout << "\nkey value:-\t" << txt->line_number;
    inorder(txt->right);
}


int main()
{  
    text_t *searchtree;
    int N=100;
   char nextop;
   searchtree = create_tree();
   int max_length = set_max_length(searchtree, 10);
   cout << "\nMade Tree:-\t";
   cout << "\nIn the following, the line_number n is associated with the object 10n+2\n 1. i: insert\n 2. d: delete \n 3. a: append\n 4. x: append to line \n 5. f: find\n 6. l: length of text\n 7. s: set text\n:-\t";
    while( (nextop = getchar())!= 'q' )
   {
     
     if( nextop == 'i' )
     {
         int insline_number, success;\

        char *insobj;
        insobj = (char *) malloc(max_length*sizeof(char));
       cin >> insline_number;
       cin >> insobj;
       insert_line( searchtree, insline_number, insobj);
       cout << "\ninsert successful, key\t" << insline_number;
       cout << "\nobject value\t" << insobj;
       cout << "\n";
       inorder(searchtree);
     }
       
     if( nextop == 'f' )
     { int findline_number;
       char *findobj;
       cin >> findline_number;
       findobj = get_line( searchtree, findline_number);
       if( findobj == NULL )
         cout << "\nfind (iterative) failed, for line_number\t" << findline_number;
       else
         cout << "\nfind (iterative) successful, found object\t" << findobj;
       cout << "\n";
     }
     
     if(nextop == 'd')
     { int delline_number;
       char *delobj;
       cin >> delline_number;
       delobj = delete_line( searchtree, delline_number);
       if( delobj == NULL )
       {
         cout << "\nfind (iterative) successful, found object\t" << delline_number;
       }
       else
       {
           cout << "\ndelete successful, deleted object\t" << delobj;
           cout << "\nline_number\t" << delline_number;
           inorder(searchtree);
       }
       cout << "\n";
     }
       
     if( nextop == 'l' )
     {  
        cout << "\nNumber of lines in the text\t";
        int total_lines;
        total_lines = length_text(searchtree);
        if( searchtree->left != NULL )
        {
            cout << "\nTotal number of lines\t" << total_lines;
        }
        else
        {
            cout << "\nEmpty Text\t";
        }
        cout << "\n";
         
     }
     if(nextop == 'a')
     {
         cout << "\nTotal number of lines\t";
         int insline_number, success;
         char *insobj;
         insobj = (char *) malloc(max_length*sizeof(char));
         cin >> insobj;
         append_line(searchtree, insobj);
         inorder(searchtree);
         if(success==0)
         {
             cout << "\nappend failed as text_length greater than max_length\t" ;
         }
         else
         {
             cout << "\nappend successful\t" ;
         }
         cout << "\n";
     }
     if(nextop == 's')
     {
         int insline_number;
         char *insobj;
         insobj = (char *) malloc(max_length*sizeof(char));
         char *previous_line;
         cin >> insline_number;
         cin >> insobj;
         
         previous_line = set_line(searchtree, insline_number, insobj);
         cout << "\nreturned from set function\t" << previous_line;
         if(previous_line==NULL)
         {
             cout << "\nset line failed as text_length greater than max_length\t";
         }
         else
         {
             cout << "\nset line successful, previously was\t" << previous_line;
             
         }
         cout << "\n";
     }
     if(nextop == 'x')
     {
         char *insobj;
         insobj = (char *) malloc(max_length*sizeof(char));
         cin >> insobj;
         append_to_line(searchtree, insobj);
         cout << "\n";
     }
     
     
   }

   remove_tree( searchtree );
   cout << "\nRemoved tree.\t";
   cout << "\nTotal number of nodes taken\t" << nodes_taken;
   cout << "\ntotal number of nodes returned\t" << nodes_returned;
   cout << "\n";
   
   return(0);
}