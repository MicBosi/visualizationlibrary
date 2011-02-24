/**************************************************************************************/
/*                                                                                    */
/*  Copyright (c) 2005-2010, Michele Bosi.                                            */
/*  All rights reserved.                                                              */
/*                                                                                    */
/*  This file is part of Visualization Library                                        */
/*  http://www.visualizationlibrary.com                                               */
/*                                                                                    */
/*  Released under the OSI approved Simplified BSD License                            */
/*  http://www.opensource.org/licenses/bsd-license.php                                */
/*                                                                                    */
/**************************************************************************************/

#extension GL_EXT_texture_array: enable
uniform sampler1DArray sampler0;
void main(void)
{
	gl_FragColor = texture1DArray(sampler0, gl_TexCoord[0].xy );
}