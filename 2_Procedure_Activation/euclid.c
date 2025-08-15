int gcd(int a, int b) {
	if(a>0 && b>0) {
	if(a<b) return gcd(a,b-a);
	else if(b<a) return gcd(b,a-b);
	else return a;
	}
	else return -1;
}
#include<stdio.h>
#include<stdlib.h>
int main(int argc, char *argv[]) {
  int a = 10, b = 15;
  if(argc>1) a = atoi(argv[1]);
  if(argc>2) b = atoi(argv[2]);
  return printf("gcd(%d,%d)=%d\n",a,b,gcd(a,b));
}
