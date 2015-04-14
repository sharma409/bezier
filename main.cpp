#include <cstdlib>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <vector>
#include <malloc.h>
#include "vector.h"
#include "bezier.h"
#include <iostream>
using std::cout;
using std::endl;

#include <fstream>
using std::ifstream;

#include <cstring>

const int MAX_CHARS_PER_LINE = 512;
const int MAX_TOKENS_PER_LINE = 12;
const char* const DELIMITER = " ";

using std::vector;

vector<Bezier*> readBEZ(char* filename){


	// create a file-reading object
	ifstream fin;
	fin.open(filename); // open a file
	if (!fin.good())
		return; // exit if file not found
	vector<Bezier*> bez;
	float coords[48];
	int line = 0;
	// read each line of the file

	
	while (!fin.eof())
	{	
		if (line == 0){
			continue;
		}
		// read an entire line into memory
		char buf[MAX_CHARS_PER_LINE];
		fin.getline(buf, MAX_CHARS_PER_LINE);

		// parse the line into blank-delimited tokens
		int n = 0; // a for-loop index

		// array to store memory addresses of the tokens in buf
		const char* token[MAX_TOKENS_PER_LINE] = {}; // initialize to 0

		// parse the line
		token[0] = strtok(buf, DELIMITER); // first token
		if (token[0]) // zero if line is blank
		{
			for (n = 1; n < MAX_TOKENS_PER_LINE; n++)
			{
				token[n] = strtok(0, DELIMITER); // subsequent tokens
				if (!token[n]) break; // no more tokens
			}
		}
		//place tokens into array of coords
		for (int i = 0; i < n; i++) // n = #of tokens
			coords[line * 4 + i] = atof(token[i]);

		// process (print) the tokens
		if (line == 4){ //process coords into bez object
			Vector* v1 = new Vector(coords[0], coords[1], coords[2]);
			Vector* v2 = new Vector(coords[12], coords[13], coords[14]);
			Vector* v3 = new Vector(coords[24], coords[25], coords[26]);
			Vector* v4 = new Vector(coords[36], coords[37], coords[38]);

			Vector* v5 = new Vector(coords[3], coords[4], coords[5]);
			Vector* v6 = new Vector(coords[15], coords[16], coords[17]);
			Vector* v7 = new Vector(coords[27], coords[28], coords[29]);
			Vector* v8 = new Vector(coords[39], coords[40], coords[41]);

			Vector* v9 = new Vector(coords[6], coords[7], coords[8]);
			Vector* v10 = new Vector(coords[18], coords[19], coords[20]);
			Vector* v11 = new Vector(coords[30], coords[31], coords[32]);
			Vector* v12 = new Vector(coords[42], coords[43], coords[44]);

			Vector* v13 = new Vector(coords[9], coords[10], coords[11]);
			Vector* v14 = new Vector(coords[21], coords[22], coords[23]);
			Vector* v15 = new Vector(coords[33], coords[34], coords[35]);
			Vector* v16 = new Vector(coords[45], coords[46], coords[47]);
		
			Vector* vectors[4][4] = {v1,v2,v3,v4,
									v5,v6,v7,v8,
									v9,v10,v11,v12,
									v13,v14,v15,v16};
			bez.push_back(new Bezier(vectors,1,1));
			line = -1;
		}
		
		line++;
	}
	return bez;
}