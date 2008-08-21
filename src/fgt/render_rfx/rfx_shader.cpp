/****************************************************************************
* MeshLab                                                           o o     *
* A versatile mesh processing toolbox                             o     o   *
*                                                                _   O  _   *
* Copyright(C) 2005-2008                                           \/)\/    *
* Visual Computing Lab                                            /\/|      *
* ISTI - Italian National Research Council                           |      *
*                                                                    \      *
* All rights reserved.                                                      *
*                                                                           *
* This program is free software; you can redistribute it and/or modify      *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation; either version 2 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
*                                                                           *
****************************************************************************/

#include "rfx_shader.h"

RfxShader::RfxShader()
{
}

RfxShader::~RfxShader()
{
	foreach (RfxGLPass *pass, shaderPasses) {
		delete pass;
		pass = 0;
	}
	shaderPasses.clear();
}

int RfxShader::FindRT(const QString& theName)
{
	for (int i = 0; i < renderTargets.size(); ++i)
		if (renderTargets.at(i)->GetName() == theName)
			return i;

	return -1;
}

void RfxShader::SortPasses()
{
	int minVal = shaderPasses.at(0)->GetPassIndex();
	int minIdx = 0;

	for (int i = 0; i < shaderPasses.size(); ++i) {
		if (shaderPasses.at(i)->GetPassIndex() < minVal) {
			shaderPasses.swap(i, minIdx);
			minIdx = i;
			minVal = shaderPasses.at(i)->GetPassIndex();
		}
	}
}

void RfxShader::CompileAndLink(QGLContext *ctx)
{
	foreach (RfxGLPass *pass, shaderPasses)
		pass->CompileAndLink(ctx);
}

void RfxShader::Start()
{
	foreach (RfxGLPass *pass, shaderPasses) {
		RfxRenderTarget *rt = NULL;
		if (pass->wantsRenderTarget()) {
			rt = pass->GetRenderTarget();
			int passIdx = pass->GetPassIndex();
			if (rt->Setup(passIdx))
				rt->Bind(passIdx);
		}

		pass->Start();

		if (rt != NULL)
			rt->Unbind();
	}
}
