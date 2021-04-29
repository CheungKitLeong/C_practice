/*
* CSCI3280 Introduction to Multimedia Systems *
* --- Declaration --- *
* I declare that the assignment here submitted is original except for source
* material explicitly acknowledged. I also acknowledge that I am aware of
* University policy and regulations on honesty in academic work, and of the
* disciplinary guidelines and procedures applicable to breaches of such policy
* and regulations, as contained in the website
* http://www.cuhk.edu.hk/policy/academichonesty/ *
* Assignment 3
* Name : Cheung Kit Leong
* Student ID : 1155143469
* Email Addr : 1155143469@link.cuhk.edu.hk
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define CODE_SIZE  12
#define TRUE 1
#define FALSE 0


/* function prototypes */
unsigned int read_code(FILE*, unsigned int); 
void write_code(FILE*, unsigned int, unsigned int); 
void writefileheader(FILE *,char**,int);
void readfileheader(FILE *,char**,int *);
void compress(FILE*, FILE*);
void decompress(FILE*, FILE*);
// Global
#define RESERVED_EOF (0x1 << CODE_SIZE)-1
#define RESERVED 256
typedef struct Node Node;
void deldict(Node* dict);
Node* dict = NULL;
unsigned int newcode = RESERVED;
char** strdict;

int main(int argc, char **argv)
{
    int printusage = 0;
    int	no_of_file;
    char **input_file_names;    
	char *output_file_names;
    FILE *lzw_file;

    if (argc >= 3)
    {
		if ( strcmp(argv[1],"-c") == 0)
		{		
			/* compression */
			lzw_file = fopen(argv[2] ,"wb");
        
			/* write the file header */
			input_file_names = argv + 3;
			no_of_file = argc - 3;
			writefileheader(lzw_file,input_file_names,no_of_file);
        	        	
			/* ADD CODES HERE */
			while(no_of_file > 0){
				FILE* input = fopen(argv[argc-no_of_file], "rb");
				if(input == NULL){
					exit(1);
				}
				compress(input, lzw_file);
				fclose(input);
				no_of_file--;
			}
			write_code(lzw_file, 0, CODE_SIZE);
        	deldict(dict);
			fclose(lzw_file);        	
		} else
		if ( strcmp(argv[1],"-d") == 0)
		{	
			/* decompress */
			lzw_file = fopen(argv[2] ,"rb");
			
			/* read the file header */
			no_of_file = 0;			
			readfileheader(lzw_file,&output_file_names,&no_of_file);
			
			/* ADD CODES HERE */
			int pos = 0;
			int namesize = 0;
			int firstc = 0;
			char* filename;
			strdict = (char**) malloc((RESERVED_EOF - RESERVED + 1) * sizeof(char*));
			while(no_of_file > 0){
				while(output_file_names[pos] != '\n'){
					pos++;
				}
				namesize = pos - firstc;
				filename = (char*) malloc(namesize + 1);
				strncpy(filename, output_file_names + firstc, namesize);
				filename[namesize] = '\0';
				FILE* output = fopen(filename, "wb");
				if(output == NULL){
					exit(1);
				}
				decompress(lzw_file, output);
				fclose(output);
				firstc = pos + 1;
				pos++;
				free(filename);
				no_of_file--;
			}
			newcode = newcode - RESERVED;
			for(int i = 0; i < newcode; i++){
				free(strdict[i]);
			}
			free(strdict);
			fclose(lzw_file);		
			free(output_file_names);
		}else
			printusage = 1;
    }else
		printusage = 1;

	if (printusage)
		printf("Usage: %s -<c/d> <lzw filename> <list of files>\n",argv[0]);
 	
	return 0;
}

/*****************************************************************
 *
 * writefileheader() -  write the lzw file header to support multiple files
 *
 ****************************************************************/
void writefileheader(FILE *lzw_file,char** input_file_names,int no_of_files)
{
	int i;
	/* write the file header */
	for ( i = 0 ; i < no_of_files; i++) 
	{
		fprintf(lzw_file,"%s\n",input_file_names[i]);	
			
	}
	fputc('\n',lzw_file);

}

/*****************************************************************
 *
 * readfileheader() - read the fileheader from the lzw file
 *
 ****************************************************************/
void readfileheader(FILE *lzw_file,char** output_filenames,int * no_of_files)
{
	int noofchar;
	char c,lastc;

	noofchar = 0;
	lastc = 0;
	*no_of_files=0;
	/* find where is the end of double newline */
	while((c = fgetc(lzw_file)) != EOF)
	{
		noofchar++;
		if (c =='\n')
		{
			if (lastc == c )
				/* found double newline */
				break;
			(*no_of_files)++;
		}
		lastc = c;
	}

	if (c == EOF)
	{
		/* problem .... file may have corrupted*/
		*no_of_files = 0;
		return;
	
	}
	/* allocate memeory for the filenames */
	*output_filenames = (char *) malloc(sizeof(char)*noofchar);
	/* roll back to start */
	fseek(lzw_file,0,SEEK_SET);

	fread((*output_filenames),1,(size_t)noofchar,lzw_file);
	
	return;
}

/*****************************************************************
 *
 * read_code() - reads a specific-size code from the code file
 *
 ****************************************************************/
unsigned int read_code(FILE *input, unsigned int code_size)
{
    unsigned int return_value;
    static int input_bit_count = 0;
    static unsigned long input_bit_buffer = 0L;

    /* The code file is treated as an input bit-stream. Each     */
    /*   character read is stored in input_bit_buffer, which     */
    /*   is 32-bit wide.                                         */

    /* input_bit_count stores the no. of bits left in the buffer */

    while (input_bit_count <= 24) {
        input_bit_buffer |= (unsigned long) getc(input) << (24-input_bit_count);
        input_bit_count += 8;
    }
    
    return_value = input_bit_buffer >> (32 - code_size);
    input_bit_buffer <<= code_size;
    input_bit_count -= code_size;
    
    return(return_value);
}


/*****************************************************************
 *
 * write_code() - write a code (of specific length) to the file 
 *
 ****************************************************************/
void write_code(FILE *output, unsigned int code, unsigned int code_size)
{
    static int output_bit_count = 0;
    static unsigned long output_bit_buffer = 0L;

    /* Each output code is first stored in output_bit_buffer,    */
    /*   which is 32-bit wide. Content in output_bit_buffer is   */
    /*   written to the output file in bytes.                    */

    /* output_bit_count stores the no. of bits left              */    

    output_bit_buffer |= (unsigned long) code << (32-code_size-output_bit_count);
    output_bit_count += code_size;

    while (output_bit_count >= 8) {
        putc(output_bit_buffer >> 24, output);
        output_bit_buffer <<= 8;
        output_bit_count -= 8;
    }


    /* only < 8 bits left in the buffer                          */    

}



/*****************************************************************
 *
 * AVL trees structure implememtation for Codedict
 *
 ****************************************************************/


// An entry in dict
typedef struct Entry{
	char* str;
	int code;
}Entry;

// AVL binary search tree
struct Node{
	Entry* ele;
	Node* left;
	Node* right;
	int height;
};

Node* create(Entry* ety){
	Node* root = (Node*) malloc(sizeof(Node));
	root->ele = ety;
	root->left = NULL;
	root->right =NULL;
	root->height = 0;
	return root;
}

int getheight(Node* root){
	if(root == NULL){
		return -1;
	}else{
		return root->height;
	}
}

int setheight(Node* root){
	return max(getheight(root->left), getheight(root->right)) + 1;
}

Node* rightrotate(Node* root){
	Node* temp = root;
	root = root->left;
	temp->left = root->right;
	root->right = temp;
	root->right->height = setheight(root->right);
	root->height = setheight(root);
	return root;
}

Node* leftrotate(Node* root){
	Node* temp = root;
	root = root->right;
	temp->right = root->left;
	root->left = temp;
	root->left->height = setheight(root->left);
	root->height = setheight(root);
	return root;
}

Node* insert(Node* root, Entry* ety){
	if(root == NULL){
		return create(ety);
	}
	int cmp = strcmp(ety->str, root->ele->str);
	if(cmp < 0){
		root->left = insert(root->left, ety);
	}else if(cmp > 0){
		root->right = insert(root->right, ety);
	}else{
		exit(1);
	}
	root->height = setheight(root);
	cmp = getheight(root->left) - getheight(root->right);
	if(cmp > 1){
		cmp = strcmp(ety->str, root->left->ele->str);
		if(cmp > 0){
			root->left = leftrotate(root->left);
		}
		return rightrotate(root);
	}else if (cmp < -1){
		cmp = strcmp(ety->str, root->right->ele->str);
		if(cmp < 0){
			root->right = rightrotate(root->right);
		}
		return leftrotate(root);
	}
	return root;
}

Entry* search(Node* root, char* str){
	if(root == NULL){
		return NULL;
	}else{
		int cmp = strcmp(str, root->ele->str);
		if(cmp == 0){
			return root->ele;
		}else if(cmp > 0){
			return search(root->right, str);
		}else{
			return search(root->left, str);
		}
	}
}

Entry* makeentry(char* str, int x, int size){
	Entry* ety = (Entry*) malloc(sizeof(Entry));
	ety->str = (char*)malloc(size);
	strcpy(ety->str, str);
	ety->code = x;
	return ety;
}

void deldict(Node* dict){
	if(dict == NULL){
		return;
	}else{
		deldict(dict->left);
		deldict(dict->right);
		free(dict->ele->str);
		free(dict->ele);
		free(dict);
		return;
	}
}
/*****************************************************************
 *
 * compress() - compress the source file and output the coded text
 *
 ****************************************************************/

void compress(FILE *input, FILE *output)
{
	/* ADD CODES HERE */
	int prefixsizelimit = 1000;
	char* p = (char*)malloc(prefixsizelimit);
	*p = '\0';
	char c;
	int pos = 0;
	unsigned int x; 
	Entry* tempx;
	while((c = fgetc(input)) != EOF){
		if(pos >= prefixsizelimit){
			prefixsizelimit *= 2;
			p = realloc(p, prefixsizelimit);
		}
		p[pos] = c;
		p[pos+1] = '\0';
		if(pos < 1){
			x = (unsigned int)p[0];
			pos++;
		}else if((tempx = search(dict, p)) == NULL){
			write_code(output, x, CODE_SIZE);
			Entry* ety = makeentry(p, newcode, pos + 2);
			dict = insert(dict, ety);
			newcode++;
			if(newcode > RESERVED_EOF){
				deldict(dict);
				newcode = RESERVED;
				dict = NULL;
			}
			pos = 1;
			p[0] = c;
			p[1] = '\0';
			x = (unsigned int)p[0];
		}else{
			x = tempx->code;
			pos++;
		}
	}
	write_code(output, x, CODE_SIZE);
	write_code(output, RESERVED_EOF, CODE_SIZE);
	free(p);
	return;
}

/*****************************************************************
 *
 * decompress() - decompress a compressed file to the orig. file
 *
 ****************************************************************/
void decompress(FILE *input, FILE *output)
{	
	/* ADD CODES HERE */
	int sizelimit = 1000;
	unsigned int cw = read_code(input, CODE_SIZE);
	char* p = (char*) malloc(sizelimit);
	int pos = 0;
	if(cw < RESERVED){
		p[0] = (char) cw;
		p[1] = '\0';
	}else{
		strcpy(p, strdict[cw-RESERVED]);
	}	
	fputs(p ,output);
	char c;
	while((cw = read_code(input, CODE_SIZE)) != RESERVED_EOF){
		if(cw < newcode){
			if(cw < RESERVED){
				c = (char) cw;
				fputc(c, output);
			}else{
				c = strdict[cw - RESERVED][0];
				fputs(strdict[cw - RESERVED], output);
			}
		}else{
			c = p[0];
			fputs(p, output);
			fputc(c, output);
		}
		pos++;
		if(pos >= sizelimit){
			sizelimit += 1000;
			p = (char*) realloc(p, sizelimit);
		}
		p[pos] = c;
		p[pos+1] = '\0';
		strdict[newcode - RESERVED] = (char*)malloc(pos + 2);
		strcpy(strdict[newcode - RESERVED], p);
		newcode++;
		if(newcode > RESERVED_EOF){
			newcode = newcode - RESERVED;
			for(int i = 0; i < newcode; i++){
				free(strdict[i]);
			}
			newcode = RESERVED;
		}
		pos = 0;
		if(cw < RESERVED){
			p[0] = (char)cw;
		}else{
			while(strdict[cw - RESERVED][pos]!='\0'){
				p[pos] = strdict[cw - RESERVED][pos];
				pos++;
			}
			pos--;
		}
		p[pos+1] = '\0';
	}
	free(p);
	return;
}
