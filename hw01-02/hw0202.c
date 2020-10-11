#include <stdio.h>
#include <stdint.h>


struct point{
	int64_t x,y;
};

struct line{
	int64_t a,b,c;
	struct point pt1,pt2;
}line1,line2;

struct line get_abc(struct line l){
	l.a=l.pt1.x-l.pt2.y;
	l.b=l.pt2.x-l.pt1.x;
	l.c=l.pt1.x*l.pt2.y-l.pt2.x*l.pt1.y;
	return l;	
}

int main(){
	int64_t delta,deltax,deltay;
	
	printf("Please enter the first line:");
	scanf(" (%ld,%ld),(%ld,%ld)",&line1.pt1.x,&line1.pt1.y,&line1.pt2.x,&line1.pt2.y);
	printf("Please enter the second line:");
	scanf(" (%ld,%ld),(%ld,%ld)",&line2.pt1.x,&line2.pt1.y,&line2.pt2.x,&line2.pt2.y);

	line1=get_abc(line1);
	line2=get_abc(line2);

	delta=line1.a*line2.b-line2.a*line1.b;
	deltax=line1.b*line2.c-line2.b*line1.c;
	deltay=line2.a*line1.c-line1.a*line2.c;
	
	if(delta==0){
		printf("Intersection does not exist.");
	}
	else{
		printf("Intersection:(%ld,%ld)\n",deltax/delta,deltay/delta);
	}


}
