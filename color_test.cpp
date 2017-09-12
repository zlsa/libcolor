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
				printf("(%.6f,%.6f,%.6f,%.6f)\n",test_a.red(),test_a.green(),test_a.blue(),test_a.alpha());
			
				test_a=Color::SRGBA(Color::RGBA(Color::HCYA(Color::RGBA(test_a))));
				printf("(%.6f,%.6f,%.6f,%.6f)\n",test_a.red(),test_a.green(),test_a.blue(),test_a.alpha());
			
				test_a=Color::SRGBA(Color::HSLA(test_a));
				printf("(%.6f,%.6f,%.6f,%.6f)\n",test_a.red(),test_a.green(),test_a.blue(),test_a.alpha());
			
				test_a=Color::SRGBA(Color::HSVA(test_a));
				printf("(%.6f,%.6f,%.6f,%.6f)\n",test_a.red(),test_a.green(),test_a.blue(),test_a.alpha());
				
				test_a=Color::SRGBA(Color::RGBA( test_a));
				printf("(%.6f,%.6f,%.6f,%.6f)\n",test_a.red(),test_a.green(),test_a.blue(),test_a.alpha());
				
				test_a=Color::SRGBA(Color::RGBA( Color::XYZA(Color::RGBA(test_a))));
				printf("(%.6f,%.6f,%.6f,%.6f)\n",test_a.red(),test_a.green(),test_a.blue(),test_a.alpha());
				
				test_a=Color::SRGBA(Color::RGBA( Color::HCIA(Color::RGBA(test_a))));
				printf("(%.6f,%.6f,%.6f,%.6f)\n",test_a.red(),test_a.green(),test_a.blue(),test_a.alpha());
				
				putchar('\n');
				}
			}
		}
	
	return 0;
	}
