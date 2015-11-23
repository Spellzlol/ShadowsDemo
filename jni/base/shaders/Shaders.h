#include <GLES2/gl2.h>

#pragma once

#define TOSTRING(shader) #shader

// ----------------------------------------------------------------------------------------------------------------------------------
// Position - Color
// ----------------------------------------------------------------------------------------------------------------------------------

const char* const PositionColorVert = TOSTRING(

		uniform mat4 u_projection;

		attribute vec4 a_position;
		attribute vec4 a_color;

		varying highp vec4 v_fragmentColor;

		void main()
		{
			v_fragmentColor = a_color;
			gl_Position = u_projection * a_position;
		}

);

const char* const PositionColorFrag = TOSTRING(
		precision highp float;
		varying vec4 v_fragmentColor;

		void main()
		{
			gl_FragColor = v_fragmentColor;
		}
		
);

// ----------------------------------------------------------------------------------------------------------------------------------
// Position - Color - Light
// ----------------------------------------------------------------------------------------------------------------------------------

const char* const PositionColorLightVert = TOSTRING(

		uniform mat4 u_projection;

		attribute vec4 a_position;
		attribute vec4 a_color;

		varying highp vec4 v_fragmentColor;
		varying highp vec4 v_fragmentPosition;

		void main()
		{
			v_fragmentColor = a_color;
			v_fragmentPosition = a_position;
			gl_Position = u_projection * a_position;
		}

);

const char* const PositionColorLightFrag = TOSTRING(
		precision highp float;
		varying vec4 v_fragmentColor;
		varying vec4 v_fragmentPosition;

		uniform vec2 u_lightPos;
		uniform float u_lightSize;

		void main()
		{
			float r = sqrt (pow(v_fragmentPosition.x - u_lightPos.x, 2.0) + pow(v_fragmentPosition.y - u_lightPos.y, 2.0));
			gl_FragColor = vec4(v_fragmentColor.rgb, v_fragmentColor.a * ( 1.0 - min(1.0, r / (u_lightSize * 0.5))));
		}
		
);

// ----------------------------------------------------------------------------------------------------------------------------------
// Position - Texture - Color
// ----------------------------------------------------------------------------------------------------------------------------------

const char* const PositionTextureColorVert = TOSTRING(

		uniform mat4 u_projection;

		attribute vec4 a_position;
		attribute vec4 a_color;
		attribute vec2 a_texture;

		varying highp vec4 v_fragmentColor;
		varying highp vec2 v_texCoord;

		void main()
		{
			v_fragmentColor = a_color;
			v_texCoord = a_texture;
			gl_Position = u_projection * a_position;
		}

);

const char* const PositionTextureColorFrag = TOSTRING(
		precision highp float;
		varying vec4 v_fragmentColor;
		varying vec2 v_texCoord;

		uniform sampler2D u_texture0;

		void main()
		{
			gl_FragColor = v_fragmentColor * texture2D(u_texture0, v_texCoord);
		}
		
);

// ----------------------------------------------------------------------------------------------------------------------------------
// Position - Texture - Color - Light
// ----------------------------------------------------------------------------------------------------------------------------------

const char* const PositionTextureColorLightVert = TOSTRING(

		uniform mat4 u_projection;

		attribute vec4 a_position;
		attribute vec4 a_color;
		attribute vec2 a_texture;

		varying highp vec4 v_fragmentColor;
		varying highp vec2 v_texCoord;
		varying highp vec4 v_fragmentPosition;

		void main()
		{
			v_fragmentColor = a_color;
			v_texCoord = a_texture;
			v_fragmentPosition = a_position;
			gl_Position = u_projection * a_position;
		}

);


const char* const PositionTextureColorLightFrag = TOSTRING(
		precision highp float;
		varying vec4 v_fragmentColor;
		varying vec2 v_texCoord;
		varying vec4 v_fragmentPosition;

		uniform sampler2D u_texture0;
		uniform vec2 u_lightPos;
		uniform float u_lightSize;

		void main()
		{
			vec4 color = v_fragmentColor * texture2D(u_texture0, v_texCoord);

			float r = sqrt (pow(v_fragmentPosition.x - u_lightPos.x, 2.0) + pow(v_fragmentPosition.y - u_lightPos.y, 2.0));
			gl_FragColor = vec4(color.rgb, color.a * ( 1.0 - min(1.0, r / (u_lightSize * 0.5))));
		}
		
);

// ----------------------------------------------------------------------------------------------------------------------------------
// Shadow Map
// ----------------------------------------------------------------------------------------------------------------------------------

const char* const ShadowMapFrag = TOSTRING(
		precision highp float;
		varying vec4 v_fragmentColor;
		varying vec2 v_texCoord;

		uniform sampler2D u_texture0;

		const float pi = 3.1415;
		const float treshhold = 0.2;

		void main()
		{
			// counter clockwise
			float angle = (1.0 - v_texCoord.x) * 2.0 * pi;
			float distance = 1.0;

			float dx = 0.0;
			float dy = 0.0;

			vec4 data;

			while (dx < 0.5 && dy < 0.5)
			{
				float x = cos(angle) * dx + 0.5;
				float y = 0.5 - sin(angle) * dy;

				data = texture2D(u_texture0, vec2(x,y));
				float dst = dx / 0.5;

				if (data.a > treshhold)
				{
					distance = min(distance, dst);
					break;
				}

				dx += 0.0019;
				dy += 0.0019;
			}
				
			gl_FragColor = vec4(data.rgb,distance);
		}
);

// ----------------------------------------------------------------------------------------------------------------------------------
// Shadow Render
// ----------------------------------------------------------------------------------------------------------------------------------

const char* const ShadowRenderFrag = TOSTRING(
		precision highp float;
		varying vec4 v_fragmentColor;
		varying vec2 v_texCoord;

		uniform sampler2D u_texture0;
		uniform sampler2D u_texture1;
		uniform float u_lightSize;
		uniform vec3 u_lightColor;

		const float pi = 3.1415;

		//1.0 if sample coord > r, otherwise 0.0
		float sample(float coord, float r) {
		    return step(r, texture2D(u_texture1, vec2(coord,0.0)).a);
		}

		void main(void) {
		    //rectangular to polar
		    vec2 norm = v_texCoord.st * 2.0 - 1.0;
		    float theta = atan(norm.y, norm.x);
		    float r = length(norm); 
		    float coord = (theta + pi) / (2.0*pi);

		    // alpha of current fragment
		    float center = sample(coord, r);        

		    // simple blur
		    float blur = (1.0/u_lightSize)  * smoothstep(0.0, 1.0, r); 
		    float sum = 0.0;

		    sum += sample(coord - 4.0*blur, r) * 0.05;
		    sum += sample(coord - 3.0*blur, r) * 0.09;
		    sum += sample(coord - 2.0*blur, r) * 0.12;
		    sum += sample(coord - 1.0*blur, r) * 0.15;

		    sum += center * 0.16;

		    sum += sample(coord + 1.0*blur, r) * 0.15;
		    sum += sample(coord + 2.0*blur, r) * 0.12;
		    sum += sample(coord + 3.0*blur, r) * 0.09;
		    sum += sample(coord + 4.0*blur, r) * 0.05;

		    gl_FragColor = v_fragmentColor * vec4(u_lightColor, sum * smoothstep(1.0, 0.0, r));
		}
);


