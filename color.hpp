#ifndef COLOR_HPP
#define COLOR_HPP

#include <cmath>
#include <algorithm>
#include <cassert>

namespace Color
	{
	typedef float vec4 __attribute__ ((vector_size (16)));

    class HCYpA;
    class HSLA;
    class HSVA;
    class RGBA;
    class YpCbCr;
    class SRGBA;

	class RGBA
		{
		public:
            inline explicit RGBA(SRGBA x) noexcept;
			
			explicit RGBA(float r,float g,float b,float a) noexcept
				{
				m_data[0]=r;
				m_data[1]=g;
				m_data[2]=b;
				m_data[3]=a;
				}
			
			RGBA()=default;
			
			float red() const noexcept
				{return m_data[0];}
				
			float green() const noexcept
				{return m_data[1];}
				
			float blue() const noexcept
				{return m_data[2];}
				
			float alpha() const noexcept
				{return m_data[3];}
				
			RGBA& red(float val) noexcept
				{
				m_data[0]=val;
				return *this;
				}
				
			RGBA& green(float val) noexcept
				{
				m_data[1]=val;
				return *this;
				}
				
			RGBA& blue(float val) noexcept
				{
				m_data[2]=val;
				return *this;
				}

			RGBA& alpha(float val) noexcept
				{
				m_data[3]=val;
				return *this;
				}
			
		private:
			static inline float from_srgb(float x) noexcept;
			vec4 m_data;
		};

    class SRGBA
		{
		public:
			inline explicit SRGBA(HSLA x) noexcept;
			inline explicit SRGBA(HCYpA x) noexcept;
			inline explicit SRGBA(HSVA x) noexcept;

            explicit SRGBA(RGBA x) noexcept:
	            SRGBA(to_srgb(x.red()),to_srgb(x.green()),to_srgb(x.blue()),x.alpha())
                {}

			inline explicit SRGBA(YpCbCr x) noexcept;
			
			explicit SRGBA(float r,float g,float b,float a) noexcept
				{
				m_data[0]=r;
				m_data[1]=g;
				m_data[2]=b;
				m_data[3]=a;
				}
			
			SRGBA()=default;
			
			float red() const noexcept
				{return m_data[0];}
				
			float green() const noexcept
				{return m_data[1];}
				
			float blue() const noexcept
				{return m_data[2];}
				
			float alpha() const noexcept
				{return m_data[3];}
				
			SRGBA& red(float val) noexcept
				{
				m_data[0]=val;
				return *this;
				}
				
			SRGBA& green(float val) noexcept
				{
				m_data[1]=val;
				return *this;
				}
				
			SRGBA& blue(float val) noexcept
				{
				m_data[2]=val;
				return *this;
				}

			SRGBA& alpha(float val) noexcept
				{
				m_data[3]=val;
				return *this;
				}
				
			float luma() const noexcept
				{
				vec4 weights{0.21f,0.72f,0.07f,0.0f};
				auto temp=m_data*weights;
				return temp[0] + temp[1] + temp[2] + temp[3];
				}
			
		private:
			static float to_srgb(float x) noexcept
				{
				if (x < 0.00031308f)
					{return 12.92f * x;}
				return 1.055f*std::pow(x,(1.0f / 2.4f) ) - 0.055f;
				}

			vec4 m_data;
		};
		
	inline RGBA::RGBA(SRGBA x) noexcept:
		RGBA(from_srgb(x.red()),from_srgb(x.green()),from_srgb(x.blue()),x.alpha())
		{}
	
	inline float RGBA::from_srgb(float x) noexcept
		{
		if(x < 0.04045f)
			{return x/12.92f;}
		return std::pow( (x + 0.055f)/(1 + 0.055f),2.4);
		}

	namespace detail
		{
		struct MmCH //For use with hexcone models
			{
			float M;
			float m;
			float C;
			float H;
			};

		static constexpr auto pi=std::acos(-1.0f);
			
		static MmCH mmch(SRGBA x)
			{
			auto ptr=reinterpret_cast<const float*>(&x);
			auto offset_max=std::max_element(ptr,ptr + 3);
			auto M=*offset_max;
			auto m=std::min(std::min(ptr[0],ptr[1]), ptr[2]);
			auto C=M - m;
			float hue_lut[3]=
				{
				 std::fmod((x.green() - x.blue())/C,6.0f) + (x.blue()>x.green()?6.0f:0.0f)
				,(x.blue() - x.red())/C + 2
				,(x.red() - x.green())/C + 4
				};
			assert(offset_max - ptr<3);
			return {M,m,C,C>0?pi*hue_lut[offset_max - ptr]/3.0f:0.0f};
			}
		
		static SRGBA from_hue_chroma_alpha(float hue,float chroma,float alpha)
			{
			assert(hue>=0 && hue<=2.0f*pi);
			auto H=3.0f*hue/pi;
			auto C=chroma;
			auto X=chroma*(1.0f - std::abs(std::fmod(H,2) - 1.0f));
			
			SRGBA rgb_lut[7]=
				{
				 SRGBA(C,X,0,alpha)
				,SRGBA(X,C,0,alpha)
				,SRGBA(0,C,X,alpha)
				,SRGBA(0,X,C,alpha)
				,SRGBA(X,0,C,alpha)
				,SRGBA(C,0,X,alpha)
				,SRGBA(C,X,0,alpha) //So we can accept hue=2 pi without mod
				};
				
			return rgb_lut[static_cast<int>(H)];
			}
		}
		
	class HCYpA
		{
		public:
			explicit HCYpA(SRGBA x)
				{
				auto temp=detail::mmch(x);
				m_data[0]=temp.H;
				m_data[1]=temp.C;
				m_data[2]=x.luma();
				m_data[3]=x.alpha();
				}
			
			explicit HCYpA(float hue,float chroma,float luma,float alpha)
				{
				m_data[0]=hue;
				m_data[1]=chroma;
				m_data[2]=luma;
				m_data[3]=alpha;
				}
			
			float hue() const noexcept
				{return m_data[0];}
			
			float chroma() const noexcept
				{return m_data[1];}
				
			float luma() const noexcept
				{return m_data[2];}
			
			float alpha() const noexcept
				{return m_data[3];}
				
			HCYpA& hue(float x) noexcept
				{
				m_data[0]=x;
				return *this;
				}
		
			HCYpA& chroma(float x) noexcept
				{
				m_data[1]=x;
				return *this;
				}
				
			HCYpA& luma(float x) noexcept
				{
				m_data[2]=x;
				return *this;
				}
			
			HCYpA& alpha(float x) noexcept
				{
				m_data[3]=x;
				return *this;
				}

			
		private:
			vec4 m_data;
		};
	
	inline SRGBA::SRGBA(HCYpA x) noexcept
		{
		*this=detail::from_hue_chroma_alpha(x.hue(),x.chroma(),x.alpha());
		auto m=x.luma() - luma();
		m_data+=vec4{m,m,m,0};
		}
		
	class HSLA
		{
		public:
			explicit HSLA(SRGBA x)
				{
				auto temp=detail::mmch(x);
				auto L=0.5f*(temp.M + temp.m);
				m_data[0]=temp.H;
				m_data[1]=(temp.C>0 && L<1.0f)?temp.C/(1.0f - std::abs(2.0f*L - 1.0f)):0.0f;
				m_data[2]=L;
				m_data[3]=x.alpha();
				}
			
			explicit HSLA(float hue,float saturation,float lightness,float alpha)
				{
				m_data[0]=hue;
				m_data[1]=saturation;
				m_data[2]=lightness;
				m_data[3]=alpha;
				}
			
			float hue() const noexcept
				{return m_data[0];}
			
			float saturation() const noexcept
				{return m_data[1];}
				
			float lightness() const noexcept
				{return m_data[2];}
			
			float alpha() const noexcept
				{return m_data[3];}
				
			HSLA& hue(float x) noexcept
				{
				m_data[0]=x;
				return *this;
				}
		
			HSLA& saturation(float x) noexcept
				{
				m_data[1]=x;
				return *this;
				}
				
			HSLA& lightness(float x) noexcept
				{
				m_data[2]=x;
				return *this;
				}
			
			HSLA& alpha(float x) noexcept
				{
				m_data[3]=x;
				return *this;
				}

			
		private:
			vec4 m_data;
		};
		
	inline SRGBA::SRGBA(HSLA x) noexcept
		{
		auto chroma=(1.0f - std::abs(2*x.lightness() - 1.0f) )*x.saturation();
		*this=detail::from_hue_chroma_alpha(x.hue(),chroma,x.alpha());
		auto m=x.lightness() - 0.5f*chroma;
		m_data+=vec4{m,m,m,0};
		}
	
	
	class HSVA
		{
		public:
			explicit HSVA(SRGBA x)
				{
				auto temp=detail::mmch(x);
				auto V=temp.M;
				m_data[0]=temp.H;
				m_data[1]=(V>0)?temp.C/V:0.0f;
				m_data[2]=V;
				m_data[3]=x.alpha();
				}
			
			explicit HSVA(float hue,float saturation,float value,float alpha)
				{
				m_data[0]=hue;
				m_data[1]=saturation;
				m_data[2]=value;
				m_data[3]=alpha;
				}
			
			float hue() const noexcept
				{return m_data[0];}
			
			float saturation() const noexcept
				{return m_data[1];}
				
			float value() const noexcept
				{return m_data[2];}
			
			float alpha() const noexcept
				{return m_data[3];}
				
			HSVA& hue(float x) noexcept
				{
				m_data[0]=x;
				return *this;
				}
		
			HSVA& saturation(float x) noexcept
				{
				m_data[1]=x;
				return *this;
				}
				
			HSVA& value(float x) noexcept
				{
				m_data[2]=x;
				return *this;
				}
			
			HSVA& alpha(float x) noexcept
				{
				m_data[3]=x;
				return *this;
				}

		private:
			vec4 m_data;
		};
	
	inline SRGBA::SRGBA(HSVA x) noexcept
		{
		auto chroma=x.value()*x.saturation();
		*this=detail::from_hue_chroma_alpha(x.hue(),chroma,x.alpha());
		auto m=x.value() - chroma;
		m_data+=vec4{m,m,m,0};
		}
	}

#endif

