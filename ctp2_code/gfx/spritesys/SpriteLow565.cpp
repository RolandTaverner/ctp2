

#include "c3.h"

#include "pixelutils.h"
#include "spriteutils.h"
#include "Sprite.h"

#include "TiledMap.h"

extern sint32		g_is565Format;
extern TiledMap		*g_tiledMap;



void Sprite::DrawLowClipped565(Pixel16 *frame, sint32 drawX, sint32 drawY, sint32 width, sint32 height,
							uint16 transparency, Pixel16 outlineColor, uint16 flags)
{
	uint8			*surfBase;

	
	sint32			surfWidth = m_surfWidth;
	sint32			surfHeight = m_surfHeight;
	sint32			surfPitch = m_surfPitch;

	sint32			xstart, xend, 
					ystart, yend;

	if (drawX < 0) 
	{
		xstart = 0 - drawX;
		drawX = 0;
	} else 
		xstart = 0;

	if (drawY < 0) 
	{
		ystart = 0 - drawY;
		drawY = 0;
	} else 
		ystart = 0;

	if (drawX >= (surfWidth - width)) 
		xend = (surfWidth - drawX);
	else 
		xend = width;

	if (drawY >= (surfHeight - height)) 
		yend = (surfHeight - drawY);
    else 
		yend = height;

	surfBase = m_surfBase + (drawY * surfPitch) + (drawX * sizeof(Pixel16));

	
	Pixel16		*destPixel;
	Pixel16		*srcPixel = (Pixel16 *)frame;

	Pixel16		*table = frame+1;
	Pixel16		*dataStart = table + height;

	register	sint32 j;
	sint32		len;

	sint32		xpos;

	
	for(j=ystart; j<yend; j++) 
	{
		if (table[j] != k_EMPTY_TABLE_ENTRY) 
		{		
			Pixel16		*rowData;
			Pixel16		tag;

			destPixel = (Pixel16 *)(surfBase + j * surfPitch);
			rowData = dataStart + table[j];
			tag = *rowData++;
			tag = tag & 0x0FFF;	
			xpos = 0;
			while ((tag & 0xF000) == 0) 
			{
				switch ((tag & 0x0F00) >> 8) 
				{
					case k_CHROMAKEY_RUN_ID	:
						 
						 len = (tag & 0x00FF);
						 
						 while (len) 
						 {
						 	len--;
						 	if ((xpos >= xstart) && (xpos < xend))
						 		destPixel++;
						 	xpos++;
						 }
						 break;

					case k_COPY_RUN_ID			: 

						 len = (tag & 0x00FF);

						
						 if(flags&k_BIT_DRAWFLAGS_SPECIAL1)
						 {
							
							if (flags & k_BIT_DRAWFLAGS_TRANSPARENCY) 
							{
						 		while (len) 
								{
						 			len--;
						 			if (xpos >= xstart && xpos < xend)
						 				*destPixel = (Pixel16)pixelutils_BlendFast_565(*rowData, *destPixel, transparency);
						 			destPixel++;
						 			rowData++;
						 			xpos++;
						 		}
							
								break;
							} 

							
						 	if (flags & k_BIT_DRAWFLAGS_FOGGED) 
							{
						 		while (len) 
								{
						 			len--;
						 			if ((xpos >= xstart) && (xpos < xend))
						 				*destPixel++ = pixelutils_Shadow_565(*rowData++);
						 			else
						 				rowData++;
						 			xpos++;
						 		}
						 		break;
							} 
							
						 	while (len) 
							{
						 		len--;
						 		
						 		if ((xpos >= xstart) && (xpos < xend))
						 			*destPixel++ = pixelutils_Desaturate_565(*rowData++);
						 		else
						 			rowData++;
						 		xpos++;
							}

							break;
						 }

						 
						 while (len) 
						 {
						 	len--;
						 	if ((xpos >= xstart) && (xpos < xend))
						 		*destPixel++ = *rowData++;
						 	else
						 		rowData++;
						 	xpos++;
						 }
						 
						 break;

					case k_SHADOW_RUN_ID		: 
						 len = (tag & 0x00FF);

						 while (len) 
						 {
						 	len--;
						 	if ((xpos >= xstart) && (xpos < xend))
						 	{
						 		*destPixel = pixelutils_Shadow_565(*destPixel);
						 		destPixel++;
						 	}
						 	xpos++;
						 }
						
						 break;
					case k_FEATHERED_RUN_ID	:
						 
						 if((xpos >= xstart) && (xpos < xend))
						 	*destPixel++ = *rowData;
						 
						 xpos++;
						 rowData++;
						
						 break;
						 
						
































 
				default:
						Assert(FALSE);
						break;
				}
				tag = *rowData++;
			}
		}
	}
}





Pixel16 test_BlendFast_565(sint32 pixel1, sint32 pixel2, sint32 blend)
{
    uint32 new_pixel=((uint32)pixel2*blend)>>5;

	uint32 sum=((uint32)pixel1+new_pixel)&0xFFFFE79C;

	Pixel16   answer=(Pixel16)(sum>>1);

	return answer;
#if 0
	sint32 rb2, g2;
	sint32 rb0, g0;

	rb2 = (pixel2 & 0xF81F);
	


	rb0 = ((rb2<<5)+((blend*((pixel1 & 0xF81F)-rb2))>>5)) & 0xF81F;

	g2 = (pixel2 & 0x07E0);
	g0 = (((g2<<5)+blend*((pixel1 & 0x07E0)-g2))>>5) & 0x07E0;

	return (Pixel16)(rb0|g0);
#endif
}





void Sprite::DrawLow565(Pixel16 *frame, sint32 drawX, sint32 drawY, sint32 width, sint32 height,
					 uint16 transparency, Pixel16 outlineColor, uint16 flags)
{
	uint8			*surfBase;

	
	sint32 surfWidth  = m_surfWidth;
	sint32 surfHeight = m_surfHeight;
	sint32 surfPitch  = m_surfPitch;


	
	
	
	
	
	

	
	
	if ((drawX < 0) || 
		(drawY < 0) || 
		(drawX >= (surfWidth-width)) || 
		(drawY >= (surfHeight-height))) 
	{
	    
		if (drawX <= -width)  return;
		if (drawY <= -height) return;
		if (drawX >= surfWidth)  return;
		if (drawY >= surfHeight) return;
		
		(this->*_DrawLowClipped)(frame, drawX, drawY, width, height, transparency, outlineColor, flags);
		return;
	}

	
	surfBase = m_surfBase + (drawY * surfPitch) + (drawX * sizeof(Pixel16));

	
	

	
	PixelAddress destAddr;
	PixelAddress rowAddr;
	
	Pixel16		*table = frame+1;
	Pixel16		*dataStart = table + height;

	register	sint32 j;
	
	
	uint32		w_len;
	Pixel16		tag;

	
	

	
	for(j=0; j<height; j++) 
	{
		if (table[j]!=k_EMPTY_TABLE_ENTRY) 
		{		
		   	destAddr.w_ptr = (Pixel16 *)(surfBase + j * surfPitch);
		  
			
			rowAddr.w_ptr  = dataStart + table[j];

		    tag = *rowAddr.w_ptr++;

			tag = tag & 0x0FFF;	
					
			while ((tag & 0xF000) == 0) 
			{
				switch ((tag & 0x0F00) >> 8) 
				{
					
				    case k_CHROMAKEY_RUN_ID	:
						 destAddr.w_ptr += (tag & 0x00FF);
						 break;
				  
					
					case k_COPY_RUN_ID			: 
						 
						 w_len = (tag & 0x00FF);

						 
						 if(flags&k_BIT_DRAWFLAGS_SPECIAL1)
						 {
							
							if (flags & k_BIT_DRAWFLAGS_TRANSPARENCY)
							{
								__BlendFast_565_16(destAddr,rowAddr,1,1,w_len,transparency);
								break; 
							}
							
							if (flags & k_BIT_DRAWFLAGS_FOGGED) 
							{
								__Shadow_565_32(destAddr,rowAddr,1,1,w_len>>1);
								__Shadow_565_16(destAddr,rowAddr,1,1,w_len&0x01);
								break; 
							}
							
							__Desaturate_565_16(destAddr,rowAddr,1,1,w_len);
						 	
							break; 
						 }

						 
						 __Copy_32(destAddr,rowAddr,1,1,w_len>>1);
						 __Copy_16(destAddr,rowAddr,1,1,w_len&0x01);
						
						 break;

					
					case k_SHADOW_RUN_ID		: 
						 
						 w_len = (tag & 0x00FF);

						 __Shadow_565_32(destAddr,1,w_len>>1);
						 __Shadow_565_16(destAddr,1,w_len&0x01);

						 break;

					
					case k_FEATHERED_RUN_ID	:

					  	 
						 
						 

						 
					  	 if (flags & k_BIT_DRAWFLAGS_TRANSPARENCY) 
					  	 {
					
					
					
					 		*destAddr.w_ptr = pixelutils_BlendFast_565(*rowAddr.w_ptr, *destAddr.w_ptr, transparency);
					 		destAddr.w_ptr++;
							rowAddr.w_ptr++;
					 	 } 
					  	 else 
					  

						 {
						  
							Pixel16 pixel = *rowAddr.w_ptr;

							if (flags & k_BIT_DRAWFLAGS_FOGGED) 
								pixel = pixelutils_Shadow_565(pixel);

							if (flags & k_BIT_DRAWFLAGS_DESATURATED) 
								pixel = pixelutils_Desaturate_565(pixel);

							if (flags & k_BIT_DRAWFLAGS_FEATHERING) 
							{
								uint16 alpha = (tag & 0x00FF);
							
							
							
							  
								*destAddr.w_ptr = pixelutils_BlendFast_565(pixel,*destAddr.w_ptr, (uint16)alpha>>3);
								destAddr.w_ptr++;
								rowAddr.w_ptr++;
							} 
							else 
							{
							  
							  
								*destAddr.w_ptr++ = pixel;
								rowAddr.w_ptr++;
							}
						 }
						break;

					
					default:
						Assert(FALSE);
						break;
				}
				tag = *rowAddr.w_ptr++;
			}
		}

		
	    
	}
}



void Sprite::DrawLowReversedClipped565(Pixel16 *frame, sint32 drawX, sint32 drawY, sint32 width, sint32 height,
									uint16 transparency, Pixel16 outlineColor, uint16 flags)
{
	uint8			*surfBase;

	
	sint32			surfWidth = m_surfWidth;
	sint32			surfHeight = m_surfHeight;
	sint32			surfPitch = m_surfPitch;

	sint32			xstart, xend, 
					ystart, yend;

	if (drawX < 0) 
	{
		xstart = 0 - drawX;
		drawX = 0;
	} else {
		xstart = 0;
	}

	if (drawY < 0) {
		ystart = 0 - drawY;
		drawY = 0;
	} else {
		ystart = 0;
	}

	if (drawX >= surfWidth - width) 
	{

		xend = (surfWidth - drawX);
	} 
	else 
	{
		xend = width;
	}

	if (drawY >= surfHeight - height) 
	{

		yend = (surfHeight - drawY);
	} 
	else 
	{
		yend = height;
	}

	surfBase = m_surfBase + (drawY * surfPitch) + (drawX * sizeof(Pixel16));

	
  	Pixel16		*destPixel;
  

	Pixel16		*table = frame+1;
	Pixel16		*dataStart = table + height;

	register	sint32 j;
	sint32		len;

	sint32		xpos;

	
	for(j=ystart; j<yend; j++) {
		if (table[j] != k_EMPTY_TABLE_ENTRY) 
		{		
			Pixel16		*rowData;
			Pixel16		tag;

			destPixel = (Pixel16 *)(surfBase + j * surfPitch + (width * 2));
			rowData = dataStart + table[j];
			tag = *rowData++;
			tag = tag & 0x0FFF;	
			xpos = width;
			while ((tag & 0xF000) == 0) {
				switch ((tag & 0x0F00) >> 8) {
					case k_CHROMAKEY_RUN_ID	:
							len = (tag & 0x00FF);
							while (len) {
								len--;
								if (xpos >= xstart && xpos < xend)
									destPixel--;
								xpos--;
							}
						break;
					case k_COPY_RUN_ID			: {
							len = (tag & 0x00FF);

							if (flags & k_BIT_DRAWFLAGS_TRANSPARENCY) {
								while (len) {
									len--;
									if (xpos >= xstart && xpos < xend)
										*destPixel = (Pixel16)pixelutils_BlendFast_565(*rowData, *destPixel, transparency);
									destPixel--;
									rowData++;
									xpos--;
								}
							} else 
								if (flags & k_BIT_DRAWFLAGS_FOGGED) {
									while (len) {
										len--;
										if (xpos >= xstart && xpos < xend)
											*destPixel-- = pixelutils_Shadow_565(*rowData++);
										else
											rowData++;
										xpos--;
									}
								} else 
									if (flags & k_BIT_DRAWFLAGS_DESATURATED) {
										while (len) {
											len--;
											if (xpos >= xstart && xpos < xend)
												*destPixel-- = pixelutils_Desaturate_565(*rowData++);
											else
												rowData++;
											xpos--;
										}
									} else {
										while (len) {
											len--;
											if (xpos >= xstart && xpos < xend)
												*destPixel-- = *rowData++;

											else
												rowData++;
											xpos--;
										}
									}
						}
						break;
					case k_SHADOW_RUN_ID		: {
							len = (tag & 0x00FF);

							while (len) {
								len--;
								if (xpos >= xstart && xpos < xend) {
									*destPixel = pixelutils_Shadow_565(*destPixel);
									destPixel--;
								}
								xpos--;
							}
						}
						break;
					case k_FEATHERED_RUN_ID	:
						if (flags & k_BIT_DRAWFLAGS_TRANSPARENCY) {
							if (xpos >= xstart && xpos < xend)
								*destPixel = (Pixel16)pixelutils_BlendFast_565(*rowData, *destPixel, transparency);
							destPixel--;
							rowData++;
							xpos--;
						} else {
							Pixel16 pixel = *rowData;
							if (flags & k_BIT_DRAWFLAGS_FOGGED) {
								pixel = pixelutils_Shadow_565(pixel);
							}
							if (flags & k_BIT_DRAWFLAGS_DESATURATED) {
								pixel = pixelutils_Desaturate_565(pixel);
							}
							if (flags & k_BIT_DRAWFLAGS_FEATHERING) {
								uint16 alpha = (tag & 0x00FF);
								if (xpos >= xstart && xpos < xend) {
									*destPixel = pixelutils_BlendFast_565(pixel, *destPixel, (uint16)alpha>>3);
									destPixel--;
								}
								rowData++;
								xpos--;
							} else {
								if (xpos >= xstart && xpos < xend)
									*destPixel-- = pixel;
								xpos--;
								rowData++;
							}
						}
						break;
					default:
						Assert(FALSE);
						break;
				}
				tag = *rowData++;
			}
		}
	}
}




void Sprite::DrawLowReversed565(Pixel16 *frame, sint32 drawX, sint32 drawY, sint32 width, sint32 height,
							 uint16 transparency, Pixel16 outlineColor, uint16 flags)
{
	uint8			*surfBase;

	
	sint32 surfWidth = m_surfWidth;
	sint32 surfHeight= m_surfHeight;
	sint32 surfPitch = m_surfPitch;

	
	
	
	
	
	

	
	
	if ((drawX < 0) || 
		(drawY < 0) || 
		(drawX >= (surfWidth - width)) || 
		(drawY >= (surfHeight-height))) 
	{
		
		if (drawX <= 0 - width)  return;
		if (drawY <= 0 - height) return;
		if (drawX >= surfWidth)  return;
		if (drawY >= surfHeight) return;

		(this->*_DrawLowReversedClipped)(frame, drawX, drawY, width, height, transparency, outlineColor, flags);
		return;
	}

	
	surfBase = m_surfBase + (drawY * surfPitch) + (drawX * sizeof(Pixel16));

	
	Pixel16		*table = frame+1;
	Pixel16		*dataStart = table + height;

	PixelAddress  destAddr;
	PixelAddress  rowAddr;
	Pixel16		  tag;

	register	sint32 len;
	sint32		j;

	
	for(j=0; j < height; j++) 
	{
		if (table[j] != k_EMPTY_TABLE_ENTRY) 
		{		
			destAddr.w_ptr = (Pixel16 *)(surfBase + j * surfPitch + (width * 2));
			rowAddr.w_ptr  = dataStart + table[j];
			
			tag = *rowAddr.w_ptr++;
			tag = tag & 0x0FFF;	

			while ((tag & 0xF000) == 0) 
			{
				switch ((tag & 0x0F00) >> 8) 
				{
					case k_CHROMAKEY_RUN_ID	:
						 destAddr.w_ptr -= (tag & 0x00FF);
						 break;
					
					case k_COPY_RUN_ID			: 
						 
						 
						 len = (tag & 0x00FF);

						 
						 if(flags&k_BIT_DRAWFLAGS_SPECIAL1)
						 {
							
							if (flags & k_BIT_DRAWFLAGS_TRANSPARENCY)
							{
								__BlendFast_565_16(destAddr,rowAddr,-1,1,len,transparency);
								break; 
							}
							
							if (flags & k_BIT_DRAWFLAGS_FOGGED) 
							{
								__Shadow_565_16(destAddr,rowAddr,-1,1,len);
								break; 
							}
							
							__Desaturate_565_16(destAddr,rowAddr,-1,1,len);
						 	
							break; 
						 }

						 
						 
						 
						 __Copy_16(destAddr,rowAddr,-1,1,len);
						
						 break;


				







































					case k_SHADOW_RUN_ID		: 
						 
						 len = (tag & 0x00FF);

						 
						 
						 __Shadow_565_32(destAddr,-1,len>>1);
						 __Shadow_565_16(destAddr,-1,len&0x01);

						 break;

					case k_FEATHERED_RUN_ID	:

					 

					 

						

						if (flags & k_BIT_DRAWFLAGS_TRANSPARENCY) 
						{
								*destAddr.w_ptr = pixelutils_BlendFast_565(*rowAddr.w_ptr, *destAddr.w_ptr, (uint16)transparency);
								destAddr.w_ptr--;
								rowAddr.w_ptr++;
						} 
						else 
						{
							Pixel16 pixel = *rowAddr.w_ptr;
							if (flags & k_BIT_DRAWFLAGS_FOGGED) {
								pixel = pixelutils_Shadow_565(pixel);
							}
							if (flags & k_BIT_DRAWFLAGS_DESATURATED) {
								pixel = pixelutils_Desaturate_565(pixel);
							}
							if (flags & k_BIT_DRAWFLAGS_FEATHERING) {
								uint16 alpha = (tag & 0x00FF);
								if (flags & k_BIT_DRAWFLAGS_DESATURATED) {
									pixel = pixelutils_Desaturate_565(pixel);
								}
								*destAddr.w_ptr = pixelutils_BlendFast_565(pixel, *destAddr.w_ptr, (uint16)alpha>>3);
								destAddr.w_ptr--;
								rowAddr.w_ptr++;
							} else {
								*destAddr.w_ptr-- = pixel;
								rowAddr.w_ptr++;
							}
						}
	 
				}
				tag = *rowAddr.w_ptr++;
			}
		}
	}
}

#define k_MEDIUM_KEY	0x4208




void Sprite::DrawScaledLow565(Pixel16 *data, sint32 x, sint32 y, sint32 destWidth, sint32 destHeight,
							 uint16 transparency, Pixel16 outlineColor, uint16 flags, BOOL reverse)
{
	uint8			*surfBase;

	
	Pixel16		emptyRow[2];

	emptyRow[0] = (Pixel16)((k_CHROMAKEY_RUN_ID << 8) | m_width);
	emptyRow[1] = (Pixel16)(k_EOLN_ID << 8);

	RECT destRect = { x, y, x + destWidth, y + destHeight };

	
	sint32 surfWidth = m_surfWidth;
	sint32 surfHeight = m_surfHeight;
	sint32 surfPitch = m_surfPitch;

	surfBase = m_surfBase + (y * surfPitch) + (x * sizeof(Pixel16));

	if (destRect.left < 0) return;
	if (destRect.top < 0) return;
	if (destRect.right > surfWidth) return;
	if (destRect.bottom > surfHeight) return;

	
	Pixel16			*destPixel;
	Pixel16			*srcPixel = (Pixel16 *)data;

	Pixel16			*table = data+1;
	Pixel16			*dataStart = table + m_height;

	
	sint32				vaccum;
	sint32				vincx, vincxy;
	sint32				vend;
	sint32				vdestpos;
	sint32				vpos1, vpos2;

	vaccum = destHeight*2 - m_height;
	vincx = destHeight*2;
	vincxy = (destHeight - m_height) * 2 ;

	vpos1 = 0;
	vpos2 = (sint32)((double)(m_height - destHeight) / (double)destHeight);

	vdestpos = y;
	vend = m_height - 1;

	
	while ( vpos1 < vend) {
		if (vaccum < 0) {
			vaccum += vincx;
		} else {
			
			Pixel16		*rowData1, *rowData2;
			Pixel16		pixel1, pixel2, pixel3, pixel4;
			Pixel16		pixel;

			sint32		haccum;
			sint32		hincx, hincxy;
			sint32		hend;
			sint32		hpos;
			sint32		hdestpos;

			
			if (table[vpos1] == k_EMPTY_TABLE_ENTRY) rowData1 = emptyRow;
			else rowData1 = dataStart + table[vpos1];	
			if (table[vpos2] == k_EMPTY_TABLE_ENTRY) rowData2 = emptyRow;
			else rowData2 = dataStart + table[vpos2];

			haccum = destWidth*2 - m_width;
			hincx = destWidth*2;
			hincxy = (destWidth - m_width) * 2;
			
			hpos = 0;
			if (reverse) hdestpos = x + destWidth;
			else hdestpos = x;
			hend = m_width-1;

			sint32		mode1;
			sint32		mode2;
			sint32		pos1 = 0;
			sint32		pos2 = 0;
			sint32		end1, 
						end2;
			sint32		alpha1, 
						alpha2;
			sint32		oldend1 = 0, 
						oldend2 = 0;

			Pixel16			firstPixel, 
							secondPixel;

			end1 = ReadTag(&mode1, &rowData1, &alpha1);
			end2 = ReadTag(&mode2, &rowData2, &alpha2);

			pixel1 = k_MEDIUM_KEY;
			pixel2 = k_MEDIUM_KEY;

			while (hpos < hend) {
				if (haccum < 0) {
					haccum += hincx;
				} else {
					haccum += hincxy;

					destPixel = (Pixel16 *)(surfBase + ((vdestpos-y) * surfPitch) + ((hdestpos-x) * 2));

					
					while (pos1 <= hpos) {
						switch (mode1) {
							case k_CHROMAKEY_RUN_ID	:
									firstPixel = k_MEDIUM_KEY;
								break;
							case k_COPY_RUN_ID			:
									if (flags & k_BIT_DRAWFLAGS_TRANSPARENCY) {
										firstPixel = pixelutils_Blend_565(*rowData1, *destPixel, transparency);
									} else 
									if (flags & k_BIT_DRAWFLAGS_FOGGED) {
										firstPixel = pixelutils_Shadow_565(*rowData1);
									} else 
									if (flags & k_BIT_DRAWFLAGS_DESATURATED) {
										firstPixel = pixelutils_Desaturate_565(*rowData1);
									} else {
										firstPixel = *rowData1;
									}
									rowData1++;
								break;
							case k_SHADOW_RUN_ID		:
									firstPixel = pixelutils_Shadow_565(*destPixel);
								break;			
							case k_FEATHERED_RUN_ID	:
									if (flags & k_BIT_DRAWFLAGS_OUTLINE) {
										firstPixel = outlineColor;
									} else {
										if (flags & k_BIT_DRAWFLAGS_TRANSPARENCY) {
											firstPixel = pixelutils_Blend_565(*rowData1, *destPixel, (uint16)transparency);
										} else {
											Pixel16 pixel = *rowData1;
											if (flags & k_BIT_DRAWFLAGS_FOGGED) {
												pixel = pixelutils_Shadow_565(pixel);
											}
											if (flags & k_BIT_DRAWFLAGS_DESATURATED) {
												pixel = pixelutils_Desaturate_565(pixel);
											}
											if (flags & k_BIT_DRAWFLAGS_FEATHERING) {
												pixel = pixelutils_Blend_565(pixel, *destPixel, (uint16)alpha1>>3);
											}
											firstPixel = pixel;
										}
									}
									rowData1++;
								break;
							default:
								Assert(mode1 == k_CHROMAKEY_RUN_ID || mode1 == k_COPY_RUN_ID || mode1 == k_SHADOW_RUN_ID || mode1 == k_FEATHERED_RUN_ID);
						}
						
						pos1++;

						if (pos1 >= end1) {
							oldend1 = end1;
							end1 = oldend1 + ReadTag(&mode1, &rowData1, &alpha1);
						}
					}

					
					while (pos2 <= hpos) {
					
						switch (mode2) {
							case k_CHROMAKEY_RUN_ID	:
									secondPixel = k_MEDIUM_KEY;
								break;
							case k_COPY_RUN_ID			:
									if (flags & k_BIT_DRAWFLAGS_TRANSPARENCY) {
										secondPixel = pixelutils_Blend_565(*rowData2, *destPixel, transparency);
									} else
									if (flags & k_BIT_DRAWFLAGS_FOGGED) {
										secondPixel = pixelutils_Shadow_565(*rowData2);
									} else
									if (flags & k_BIT_DRAWFLAGS_DESATURATED) {
										secondPixel = pixelutils_Desaturate_565(*rowData2);
									} else {
										secondPixel = *rowData2;
									}

									rowData2++;
								break;
							case k_SHADOW_RUN_ID		:
									secondPixel = pixelutils_Shadow_565(*destPixel);
								break;
							case k_FEATHERED_RUN_ID	:
									if (flags & k_BIT_DRAWFLAGS_OUTLINE) {
										secondPixel = outlineColor;
									} else {
										if (flags & k_BIT_DRAWFLAGS_TRANSPARENCY) {
											secondPixel = pixelutils_Blend_565(*rowData2, *destPixel, (uint16)transparency);
										} else {
											Pixel16 pixel = *rowData2;
											if (flags & k_BIT_DRAWFLAGS_FOGGED) {
												pixel = pixelutils_Shadow_565(pixel);
											}
											if (flags & k_BIT_DRAWFLAGS_DESATURATED) {
												pixel = pixelutils_Desaturate_565(pixel);
											}
											if (flags & k_BIT_DRAWFLAGS_FEATHERING) {
												pixel = pixelutils_Blend_565(pixel, *destPixel, (uint16)alpha2>>3);
											}
											secondPixel = pixel;
										}
									}
									rowData2++;
								break;
							default : 
								Assert(mode2 == k_CHROMAKEY_RUN_ID || mode2 == k_COPY_RUN_ID || mode2 == k_SHADOW_RUN_ID || mode2 == k_FEATHERED_RUN_ID);
						}

						pos2++;

						if (pos2 >= end2) {
							oldend2 = end2;
							end2 = oldend2 + ReadTag(&mode2, &rowData2, &alpha2);
						}
					}

					pixel3 = firstPixel;
					pixel4 = secondPixel;





					
					if (flags & k_BIT_DRAWFLAGS_AA) {
						if (flags & k_BIT_DRAWFLAGS_AA_4_WAY) {
							if (pixel1 != k_MEDIUM_KEY || pixel2 != k_MEDIUM_KEY || pixel3 != k_MEDIUM_KEY || pixel4 != k_MEDIUM_KEY) {
								pixel =	average_565(pixel1, pixel2, pixel3, pixel4);
								
								if (pixel == 0) 
									pixel = 0x0001;
								*destPixel = pixel;
							}
						} else {
							if (pixel1 != k_MEDIUM_KEY || pixel2 != k_MEDIUM_KEY) {
								pixel = average_565(pixel2, pixel3);
								if (pixel == 0) 
									pixel = 0x0001;
								
								*destPixel = pixel;
							}
						}
					} else {
						if (pixel3 == 0) 
							pixel3 = 0x0001;
						
						if (pixel3 != k_MEDIUM_KEY) 
							*destPixel = pixel3;
					}
					pixel1 = pixel3;
					pixel2 = pixel4;

					if (reverse) hdestpos--;
					else hdestpos++;
				}
				hpos++;
			}

			vaccum += vincxy;
			vdestpos++;
		} 
		vpos1++;
		vpos2++;
	}

	


}




void Sprite::DrawFlashLow565(Pixel16 *frame, sint32 drawX, sint32 drawY, sint32 width, sint32 height,
					 uint16 transparency, Pixel16 outlineColor, uint16 flags)
{
	uint8			*surfBase;

	RECT destRect = { drawX, drawY, drawX + width, drawY + height };

	
	sint32 surfWidth = m_surfWidth;
	sint32 surfHeight = m_surfHeight;
	sint32 surfPitch = m_surfPitch;

	surfBase = m_surfBase + (drawY * surfPitch) + (drawX * sizeof(Pixel16));

	if (destRect.left < 0) return;
	if (destRect.top < 0) return;
	if (destRect.right > surfWidth) return;
	if (destRect.bottom > surfHeight) return;

	
	Pixel16	*destPixel;
	Pixel16  *srcPixel = (Pixel16 *)frame;

	Pixel16		*table = frame+1;
	Pixel16		*dataStart = table + height;

	register sint32 j;
	register sint32 len;

	
	for(j=0; j<height; j++) 
	{
		if (table[j] != k_EMPTY_TABLE_ENTRY) 
		{		
			Pixel16		*rowData;
			Pixel16		tag;

			destPixel = (Pixel16 *)(surfBase + j * surfPitch);
			rowData = dataStart + table[j];
			tag = *rowData++;
			tag = tag & 0x0FFF;	

			while ((tag & 0xF000) == 0) 
			{
				switch ((tag & 0x0F00) >> 8) 
				{
					case k_CHROMAKEY_RUN_ID	:
						 destPixel += (tag & 0x00FF);
						 break;
			   
					case k_COPY_RUN_ID			: 

						 len = (tag & 0x00FF);

						 while (len--) 
						 {

						 	*destPixel = pixelutils_Additive_565(*destPixel, *rowData);
						 	destPixel++;
						 	rowData++;
						 }
						 break;
				  
					case k_SHADOW_RUN_ID		: 
						 len = (tag & 0x00FF);

						 while (len--) 
						 {

							*destPixel = pixelutils_Shadow_565(*destPixel);
							destPixel++;
						 }
						break;
					case k_FEATHERED_RUN_ID	:
						 *destPixel = pixelutils_Additive_565(*destPixel, *rowData);
						 rowData++;
						 destPixel++;
						 break;
					default:
						Assert(FALSE);
				}
				tag = *rowData++;
			}
		}
	}



}




void Sprite::DrawFlashLowReversed565(Pixel16 *frame, sint32 drawX, sint32 drawY, sint32 width, sint32 height,
							 uint16 transparency, Pixel16 outlineColor, uint16 flags)
{
	uint8		*surfBase;

	RECT destRect = { drawX, drawY, drawX + width, drawY + height };

	
	sint32 surfWidth = m_surfWidth;
	sint32 surfHeight = m_surfHeight;
	sint32 surfPitch = m_surfPitch;

	surfBase = m_surfBase + (drawY * surfPitch) + (drawX * sizeof(Pixel16));

	if (destRect.left < 0) return;
	if (destRect.top < 0) return;
	if (destRect.right > surfWidth) return;
	if (destRect.bottom > surfHeight) return;

	
	Pixel16	*destPixel;
	Pixel16  *srcPixel = (Pixel16 *)frame;

	Pixel16		*table = frame+1;
	Pixel16		*dataStart = table + height;

	
	for(sint32 j=0; j < height; j++) {
		if (table[j] != k_EMPTY_TABLE_ENTRY) {		
			Pixel16		*rowData;
			Pixel16		tag;

			destPixel = (Pixel16 *)(surfBase + j * surfPitch + (width * 2));

			rowData = dataStart + table[j];
			
			tag = *rowData++;
			
			tag = tag & 0x0FFF;	
					
			while ((tag & 0xF000) == 0) {
				switch ((tag & 0x0F00) >> 8) {
					case k_CHROMAKEY_RUN_ID	:
							destPixel -= (tag & 0x00FF);
						break;
					case k_COPY_RUN_ID			: {
							uint16 len = (tag & 0x00FF);

							if (flags & k_BIT_DRAWFLAGS_TRANSPARENCY) {
								for (uint16 i=0; i<len; i++) {
									*destPixel = (Pixel16)pixelutils_Blend_565(*rowData, *destPixel, transparency);
									destPixel--;
									rowData++;
								}
							} else {
								for (uint16 i=0; i<len; i++) {
									*destPixel = pixelutils_Additive_565(*rowData, *destPixel);
									destPixel--;
									rowData++;
								}
							}
						}
						break;
					case k_SHADOW_RUN_ID		: {
							uint16 len = (tag & 0x00FF);

							for (uint16 i=0; i<len; i++) {
								*destPixel = pixelutils_Shadow_565(*destPixel);
								destPixel--;
							}
						}
						break;
					case k_FEATHERED_RUN_ID	:
						if (flags & k_BIT_DRAWFLAGS_OUTLINE) {
							*destPixel-- = outlineColor;
							rowData++;
						} else {
							if (flags & k_BIT_DRAWFLAGS_TRANSPARENCY) {
									*destPixel = pixelutils_Blend_565(*rowData, *destPixel, (uint16)transparency);
									destPixel--;
									rowData++;
							} else {
								if (flags & k_BIT_DRAWFLAGS_FEATHERING) {
									uint16 alpha = (tag & 0x00FF);


									*destPixel = pixelutils_Blend_565(*rowData, *destPixel, (uint16)alpha>>3);
									destPixel--;
									rowData++;
								} else {
									*destPixel-- = *rowData++;
								}
							}
						}
						break;
				}
				tag = *rowData++;
			}
		}
	}



}

#define k_MEDIUM_KEY	0x4208




void Sprite::DrawFlashScaledLow565(Pixel16 *data, sint32 x, sint32 y, sint32 destWidth, sint32 destHeight,
							 uint16 transparency, Pixel16 outlineColor, uint16 flags, BOOL reverse)
{
	uint8			*surfBase;

	
	Pixel16		emptyRow[2];

	emptyRow[0] = (Pixel16)((k_CHROMAKEY_RUN_ID << 8) | m_width);
	emptyRow[1] = (Pixel16)(k_EOLN_ID << 8);

	RECT destRect = { x, y, x + destWidth, y + destHeight };

	
	sint32 surfWidth = m_surfWidth;
	sint32 surfHeight = m_surfHeight;
	sint32 surfPitch = m_surfPitch;

	surfBase = m_surfBase + (y * surfPitch) + (x * sizeof(Pixel16));

	if (destRect.left < 0) return;
	if (destRect.top < 0) return;
	if (destRect.right > surfWidth) return;
	if (destRect.bottom > surfHeight) return;

	
	Pixel16			*destPixel;
	Pixel16			*srcPixel = (Pixel16 *)data;

	Pixel16			*table = data+1;
	Pixel16			*dataStart = table + m_height;

	
	sint32				vaccum;
	sint32				vincx, vincxy;
	sint32				vend;
	sint32				vdestpos;
	sint32				vpos1, vpos2;

	vaccum = destHeight*2 - m_height;
	vincx = destHeight*2;
	vincxy = (destHeight - m_height) * 2 ;

	vpos1 = 0;
	vpos2 = (sint32)((double)(m_height - destHeight) / (double)destHeight);

	vdestpos = y;
	vend = m_height - 1;

	
	while ( vpos1 < vend) {
		if (vaccum < 0) {
			vaccum += vincx;
		} else {
			
			Pixel16		*rowData1, *rowData2;
			Pixel16		pixel1, pixel2, pixel3, pixel4;
			Pixel16		pixel;

			sint32		haccum;
			sint32		hincx, hincxy;
			sint32		hend;
			sint32		hpos;
			sint32		hdestpos;

			
			if (table[vpos1] == k_EMPTY_TABLE_ENTRY) rowData1 = emptyRow;
			else rowData1 = dataStart + table[vpos1];	
			if (table[vpos2] == k_EMPTY_TABLE_ENTRY) rowData2 = emptyRow;
			else rowData2 = dataStart + table[vpos2];

			haccum = destWidth*2 - m_width;
			hincx = destWidth*2;
			hincxy = (destWidth - m_width) * 2;
			
			hpos = 0;
			if (reverse) hdestpos = x + destWidth;
			else hdestpos = x;
			hend = m_width-1;

			


			sint32		mode1;
			sint32		mode2;
			sint32		pos1 = 0;
			sint32		pos2 = 0;
			sint32		end1, 
						end2;
			sint32		alpha1, 
						alpha2;
			sint32		oldend1 = 0, 
						oldend2 = 0;

			Pixel16			firstPixel, 
							secondPixel;

			end1 = ReadTag(&mode1, &rowData1, &alpha1);
			end2 = ReadTag(&mode2, &rowData2, &alpha2);

			pixel1 = k_MEDIUM_KEY;
			pixel2 = k_MEDIUM_KEY;





			while (hpos < hend) {
				if (haccum < 0) {
					haccum += hincx;
				} else {
					haccum += hincxy;

					destPixel = (Pixel16 *)(surfBase + ((vdestpos-y) * surfPitch) + ((hdestpos-x) * 2));
		
					






					
					while (pos1 <= hpos) {
						switch (mode1) {
							case k_CHROMAKEY_RUN_ID	:
									firstPixel = k_MEDIUM_KEY;
								break;
							case k_COPY_RUN_ID			:
									if (flags & k_BIT_DRAWFLAGS_TRANSPARENCY) {
										firstPixel = pixelutils_Blend_565(*rowData1, *destPixel, transparency);
									} else {
										firstPixel = pixelutils_Additive_565(*rowData1, *destPixel);
									}
									rowData1++;
								break;
							case k_SHADOW_RUN_ID		:
									firstPixel = pixelutils_Shadow_565(*destPixel);
								break;			
							case k_FEATHERED_RUN_ID	:
									if (flags & k_BIT_DRAWFLAGS_OUTLINE) {
										firstPixel = outlineColor;
									} else {
										if (flags & k_BIT_DRAWFLAGS_TRANSPARENCY) {
											firstPixel = pixelutils_Blend_565(*rowData1, *destPixel, (uint16)transparency);
										} else {
											if (flags & k_BIT_DRAWFLAGS_FEATHERING) {

												firstPixel = pixelutils_Blend_565(*rowData1, *destPixel, (uint16)alpha1>>3);
											} else {
												firstPixel = *rowData1;
											}
										}
									}
									rowData1++;
								break;
							default:
								Assert(mode1 == k_CHROMAKEY_RUN_ID || mode1 == k_COPY_RUN_ID || mode1 == k_SHADOW_RUN_ID || mode1 == k_FEATHERED_RUN_ID);
						}
						
						pos1++;

						if (pos1 >= end1) {
							oldend1 = end1;
							end1 = oldend1 + ReadTag(&mode1, &rowData1, &alpha1);
						}
					}

					
					while (pos2 <= hpos) {
					
						switch (mode2) {
							case k_CHROMAKEY_RUN_ID	:
									secondPixel = k_MEDIUM_KEY;
								break;
							case k_COPY_RUN_ID			:
									if (flags & k_BIT_DRAWFLAGS_TRANSPARENCY) {
										secondPixel = pixelutils_Blend_565(*rowData2, *destPixel, transparency);
									} else {
										secondPixel = pixelutils_Additive_565(*rowData2, *destPixel);
									}
									rowData2++;
								break;
							case k_SHADOW_RUN_ID		:
									secondPixel = pixelutils_Shadow_565(*destPixel);
								break;
							case k_FEATHERED_RUN_ID	:
									if (flags & k_BIT_DRAWFLAGS_OUTLINE) {
										secondPixel = outlineColor;
									} else {
										if (flags & k_BIT_DRAWFLAGS_TRANSPARENCY) {
											secondPixel = pixelutils_Blend_565(*rowData2, *destPixel, (uint16)transparency);
										} else {
											if (flags & k_BIT_DRAWFLAGS_FEATHERING) {
												secondPixel = pixelutils_Blend_565(*rowData2, *destPixel, (uint16)alpha2>>3);

											} else {
												secondPixel = *rowData2;
											}
										}
									}
									rowData2++;
								break;
							default : 
								Assert(mode2 == k_CHROMAKEY_RUN_ID || mode2 == k_COPY_RUN_ID || mode2 == k_SHADOW_RUN_ID || mode2 == k_FEATHERED_RUN_ID);
						}

						pos2++;

						if (pos2 >= end2) {
							oldend2 = end2;
							end2 = oldend2 + ReadTag(&mode2, &rowData2, &alpha2);
						}
					}

					pixel3 = firstPixel;
					pixel4 = secondPixel;





					
					if (flags & k_BIT_DRAWFLAGS_AA) {
						if (flags & k_BIT_DRAWFLAGS_AA_4_WAY) {
							if (pixel1 != k_MEDIUM_KEY || pixel2 != k_MEDIUM_KEY || pixel3 != k_MEDIUM_KEY || pixel4 != k_MEDIUM_KEY) {
								pixel =	average_565(pixel1, pixel2, pixel3, pixel4);
								
								*destPixel = pixel;
							}
						} else {
							if (pixel1 != k_MEDIUM_KEY || pixel2 != k_MEDIUM_KEY) {
								pixel = average_565(pixel2, pixel3);
								
								*destPixel = pixel;
							}
						}
					} else {
						
						if (pixel3 != k_MEDIUM_KEY) 
							*destPixel = pixel3;
					}
					pixel1 = pixel3;
					pixel2 = pixel4;

					if (reverse) hdestpos--;
					else hdestpos++;
				}
				hpos++;
			}

			vaccum += vincxy;
			vdestpos++;
		} 
		vpos1++;
		vpos2++;
	}

	


}

#define k_REFLECTION_BLEND_LEVEL	4

void Sprite::DrawReflectionLow565(Pixel16 *frame, sint32 drawX, sint32 drawY, sint32 width, sint32 height,
					 uint16 transparency, Pixel16 outlineColor, uint16 flags)
{
	uint8			*surfBase;

	RECT destRect = { drawX, drawY, drawX + width, drawY + height };

	
	sint32 surfWidth = m_surfWidth;
	sint32 surfHeight = m_surfHeight;
	sint32 surfPitch = m_surfPitch;

	surfBase = m_surfBase + (drawY * surfPitch) + (drawX * sizeof(Pixel16));

	if (destRect.left < 0) return;
	if (destRect.top < 0) return;
	if (destRect.right > surfWidth) return;
	if (destRect.bottom > surfHeight) return;

	
	Pixel16	*destPixel;
	Pixel16  *srcPixel = (Pixel16 *)frame;

	Pixel16		*table = frame+1;
	Pixel16		*dataStart = table + height;

	
	for(sint32 j=0; j<height; j++) {
		if (table[j] != k_EMPTY_TABLE_ENTRY) {		
			Pixel16		*rowData;
			Pixel16		tag;

			destPixel = (Pixel16 *)(surfBase + (height + (height-j)/2)* surfPitch);

			rowData = dataStart + table[j];
			
			tag = *rowData++;
			
			tag = tag & 0x0FFF;	
					
			while ((tag & 0xF000) == 0) {
				switch ((tag & 0x0F00) >> 8) {
					case k_CHROMAKEY_RUN_ID	:
							destPixel += (tag & 0x00FF);

						break;
					case k_COPY_RUN_ID			: {
							uint16 len = (tag & 0x00FF);

							if (flags & k_BIT_DRAWFLAGS_TRANSPARENCY) {
								for (uint16 i=0; i<len; i++) {
									*destPixel = (Pixel16)pixelutils_BlendFast_565(*rowData, *destPixel, k_REFLECTION_BLEND_LEVEL);
									destPixel++;
									rowData++;
								}
							} else {
								for (uint16 i=0; i<len; i++) {
									*destPixel = (Pixel16)pixelutils_BlendFast_565(*rowData, *destPixel, k_REFLECTION_BLEND_LEVEL);

									destPixel++;
									rowData++;
								}
							}
						}
						break;
					case k_SHADOW_RUN_ID		: {
							uint16 len = (tag & 0x00FF);
							destPixel+=len;
						}
						break;
					case k_FEATHERED_RUN_ID	:
							*destPixel = (Pixel16)pixelutils_BlendFast_565(*rowData, *destPixel, k_REFLECTION_BLEND_LEVEL);
							destPixel++;
							rowData++;
						break;
					default:

						Assert(FALSE);
                        break;
				}
				tag = *rowData++;
			}
		}
	}
}




inline Pixel16 Sprite::average_565(Pixel16 pixel1, Pixel16 pixel2, Pixel16 pixel3, Pixel16 pixel4)
{

	uint32 sum=(uint32)(pixel1+pixel2+pixel3+pixel4)&0xE79CE79C;

	return (Pixel16)(sum>>2);





























}




inline Pixel16 Sprite::average_565(Pixel16 pixel1, Pixel16 pixel2)
{
	uint32 sum=(uint32)(pixel1+pixel2)&0xFFFFE79C;

	return (Pixel16)(sum>>1);

 


















}




inline void Sprite::__Desaturate_565_16	(PixelAddress &dest,PixelAddress &src,sint32 dest_inc,sint32 src_inc,sint32 num)
{
	while (num) 
	{
		num--;
		*dest.w_ptr=pixelutils_Desaturate_565(*src.w_ptr);
		dest.w_ptr +=dest_inc;
		src.w_ptr  +=src_inc;
	}
}



inline void Sprite::__BlendFast_565_16	(PixelAddress &dest,PixelAddress &src,sint32 dest_inc,sint32 src_inc,sint32 num,uint16 transparency)
{
	while (num) 
	{
		num--;

		*dest.w_ptr=(Pixel16)pixelutils_BlendFast_565(*src.w_ptr, *dest.w_ptr, transparency);
		
		dest.w_ptr +=dest_inc;
		src.w_ptr  +=src_inc;
	}
}





inline void Sprite::__Shadow_565_16(PixelAddress &dest,PixelAddress &src,sint32 dest_inc,sint32 src_inc,sint32 num)
{
  while (num) 
  {
  	 num--;
  	*dest.w_ptr = pixelutils_Shadow_565(*src.w_ptr);
  	 dest.w_ptr+=dest_inc;
  	 src.w_ptr +=src_inc;
  }
}




inline void Sprite::__Shadow_565_16(PixelAddress &dest,sint32 dest_inc,sint32 num)
{
  while (num) 
  {
  	 num--;
  	*dest.w_ptr = pixelutils_Shadow_565(*dest.w_ptr);
  	 dest.w_ptr+=dest_inc;
  }
}





inline void Sprite::__Shadow_565_32(PixelAddress &dest,PixelAddress &src,sint32 dest_inc,sint32 src_inc,sint32 num)
{
  while (num) 
  {
  	 num--;
  	*dest.l_ptr = pixelutils_Shadow32_565(*src.l_ptr);
  	 dest.l_ptr+=dest_inc;
  	 src.l_ptr +=src_inc;
  }
}




inline void Sprite::__Shadow_565_32(PixelAddress &dest,sint32 dest_inc,sint32 num)
{
  while (num) 
  {
  	 num--;
  	*dest.l_ptr = pixelutils_Shadow32_565(*dest.l_ptr);
  	 dest.l_ptr+=dest_inc;
  }
}


