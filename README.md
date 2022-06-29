# Text-Editor

### Description
Implemented functions to support a text editor where you use a representation of the text in which you can insert, append or delete something without copying all characters beyond the modification point. Structure supports the following variables and operations.

- text_t * create_text() creates an empty text, whose length is 0.
- int length_text( text_t *txt) returns the number of lines of the current text.
- char * get_line( text_t *txt, int index) gets the line of number index, if such a line exists, and
returns NULL else.
- void append_line( text_t *txt, char * new_line) appends new line as new last line if less than
maxlength, otherwise it returns null.
- char * set_line( text_t *txt, int index, char * new_line) sets the line of number index, if such
a line exists, to new line, and returns a pointer to the previous line of that number. If no line of
that number exists, it does not change the structure and returns NULL.
- void insert_line( text_t *txt, int index, char * new_line) inserts the line before the line of
number index, if such a line exists, to new line, renumbering all lines after that line. If no such
line exists, it appends new line as new last line. If the length of line is greater than maxlength, it
is rejected and NULL is returned.
- char * delete_line( text_t *txt, int index) deletes the line of number index, renumbering all
lines after that line, and returns a pointer to the deleted line.

In addition, we would like to allow extension of existing lines in text with substrings (subject to
such the extended string still maintaining our maxlength constraint). Specifically, a given
substring, it should be appended to the first line in the text that has enough space to accommodate the string extension without violating the maxlength constraint. For example, assume that the our
line maxlength is 100 and that our text has line1(70), line2(80), line3(40), line4(20) – the
numbers represent the string lengths. Then, given a substring of length 35, the first line number
with available space to append that substring while not overshooting the maxlength is line3. If no,
line is available to accommodate our string as an extension, we should just insert it as a brand new
line at the end of the file. The method for this function should have the following signature.

- void append_to_line( text_t *txt, char * new_substring) appends substring to the first line in
the text that has enough space to accommodate the substring (i.e. length of existing line + length
of substring ≤ maxlength). Otherwise, it appends substring as a new last line.

### Approach

Implemented self-balancing Binary Search Tree data structure to perform these operations in logarithmic time. All the data is stored at leaf nodes and internal nodes are just used for search navigation. Every node in search tree has 4 parameters associated with it.
- Left Child : Pointer to Left Child
- Right Child: Pointer to Right Child
- Index: Search key used to navigate in tree (Value is number of leaf nodes in its left subtree.)
- Height: Height of that node 

For leaf node, right child will be null and left child will refer to data.
