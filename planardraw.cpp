#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cairo/cairo.h>
#include <vector>
#include <string>
#include <algorithm>

class Vertex {
public:
	int x;
	int y;
	Vertex() {
		x=y=0;
	}
	Vertex(int _x,int _y) {
		x=_x;
		y=_y;
	}
};
class Edge {
public:
	unsigned int v1;
	unsigned int v2;
	Edge() {
		v1=v2=0;
	}
	Edge(unsigned int _v1,unsigned int _v2) {
		v1=_v1;
		v2=_v2;
	}
};

class Board {
public:
	int width;
	int height;

	std::vector<Vertex> vertices;
	std::vector<Edge> edges;

	void clear() {
		vertices.clear();
		edges.clear();
	}
	Board()	{
		width=height=800;
	}
	bool draw(std::string filename) {

		int padding=30;
		int offset_x=0;
		int offset_y=0;

		int min_x=0;
		int min_y=0;
		int max_x=0;
		int max_y=0;
		for(int i=0;i<vertices.size();i++) {
			if(i==0) {
				min_x=max_x=vertices[i].x;
				min_y=min_y=vertices[i].y;
			}
			else {
				min_x=std::min(min_x,vertices[i].x);
				min_y=std::min(min_y,vertices[i].y);
				max_x=std::max(max_x,vertices[i].x);
				max_y=std::max(max_y,vertices[i].y);
			}
		}
		width=max_x-min_x+padding*2;
		height=max_y-min_y+padding*2;
		offset_x=-min_x+padding;
		offset_y=-min_y+padding;


		bool success=true;
		cairo_surface_t* target=cairo_image_surface_create(CAIRO_FORMAT_ARGB32,width,height);
		cairo_t* cr=cairo_create(target);

		cairo_set_source_rgba(cr,1,1,1,1);
		cairo_rectangle(cr,0,0,width,height);
		cairo_fill(cr);

		cairo_translate(cr,offset_x,offset_y);

		cairo_set_line_width(cr,2.0);
		cairo_set_source_rgba(cr,0.5,0.5,0.5,1.0);
		for(int i=0;i<edges.size();i++) {
			cairo_move_to(cr,vertices[edges[i].v1].x,vertices[edges[i].v1].y);
			cairo_line_to(cr,vertices[edges[i].v2].x,vertices[edges[i].v2].y);
			cairo_stroke(cr);
		}

		for(int i=0;i<vertices.size();i++) {
			cairo_arc(cr,vertices[i].x,vertices[i].y,8,0,2*M_PI);
			cairo_set_source_rgba(cr,0.2,0.2,1.0,1.0);
			cairo_fill_preserve(cr);
			cairo_set_source_rgba(cr,0,0,0,1);
			cairo_stroke(cr);
		}

		if(cairo_surface_write_to_png(target,filename.c_str())!=CAIRO_STATUS_SUCCESS) {
			success=false;
		}

		cairo_destroy(cr);
		cairo_surface_destroy(target);

		return success;
	}
};
class Parser {
	static const int key_length=100;
	int key_pos;
	char key[100+1];
	FILE* file;

	enum Key {
		KEY_SCORING,
		KEY_BOARD,
		KEY_VERTEX,
		KEY_EDGE,
		KEY_SHOW_INTERSECTIONS,
		KEY_UNKNOWN
	};

	Key strToKey(std::string key) {
		if(key=="scoring") return KEY_SCORING;
		if(key=="board") return KEY_BOARD;
		if(key=="vertex") return KEY_VERTEX;
		if(key=="edge") return KEY_EDGE;
		if(key=="show_intersections") return KEY_SHOW_INTERSECTIONS;
		return KEY_UNKNOWN;
	}
	bool readKey(int count=1) {
		for(int i=0;i<count;i++) {
			key_pos=0;
			while(key_pos<=key_length) {
				int c=fgetc(file);
				if(c==EOF || c==' ' || c=='\n' || c=='\t') {
					if(c!=EOF && key_pos==0) {
						continue;
					}
					else {
						break;
					}
				}
				key[key_pos]=c;
				key_pos++;
			}
			key[key_pos]=0;
		}
		return (key_pos>0);
	}
	int readInt() {
		if(!readKey()) {
			return 0;
		}
		return atoi(key);
	}

public:
	bool load(Board& board,std::string filename) {
		board.clear();

		file=fopen(filename.c_str(),"rb");
		if(!file) {
			printf("can't open file %s\n",filename.c_str());
			return false;
		}
		bool have_error=false;
		while(!have_error && readKey()) {
			switch(strToKey(key)) {
				case KEY_SCORING:
					readKey(6);
					break;
				case KEY_BOARD:
					board.width=readInt();
					board.height=readInt();
					readKey(2);
					break;
				case KEY_VERTEX: {
					readKey(2);
					int x=readInt();
					int y=readInt();
					board.vertices.push_back(Vertex(x,y));
					readKey(1);
					break;
				}
				case KEY_EDGE: {
					int v1=readInt();
					int v2=readInt();
					board.edges.push_back(Edge(v1,v2));
					break;
				}
				case KEY_SHOW_INTERSECTIONS:
					readKey();
					break;
				default:
					printf("error parsing file %s, key '%s'\n",filename.c_str(),key);
					have_error=true;
					break;
			}
		}
		fclose(file);
		file=NULL;

		return (!have_error);
	}
};

int main(int argc,char** argv) {
	if(argc!=3) {
		printf("%s input output\n",argv[0]);
		return 1;
	}

	Board board;
	Parser parser;
	if(!parser.load(board,argv[1])) {
		return 1;
	}
	if(!board.draw(argv[2])) {
		printf("can't write to %s\n",argv[2]);
		return 1;
	}
	return 0;
}


