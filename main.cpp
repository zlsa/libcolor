//@	{"targets":[{"name":"test","type":"application"}]}

#include "color.hpp"

int main()
	{

	
	for(int k=0;k<5;++k)
		{
		for(int l=0;l<5;++l)
			{
			for(int m=0;m<5;++m)
				{
				Color::SRGBA test_a(float(k)/4.0f,float(l)/4.0f,float(m)/4.0f,1.0f);
				Color::HCYpA test_b(test_a);
				printf("(%.7f,%.7f,%.7f,%.7f) ",test_a.red(),test_a.green(),test_a.blue(),test_a.alpha());
				Color::SRGBA test_c(test_b);
				printf("(%.7f,%.7f,%.7f,%.7f) ",test_c.red(),test_c.green(),test_c.blue(),test_c.alpha());
				printf("(%.7f,%.7f,%.7f,%.7f)",test_b.hue(),test_b.chroma(),test_b.luma(),test_b.alpha());
				putchar('\n');
				}
			}
		}
	
	return 0;
	}
