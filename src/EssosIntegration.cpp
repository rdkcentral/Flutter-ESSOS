/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2020 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "EssosIntegration.h"

#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>

static EssTerminateListener terminateListener =
{
    //terminated
    [](void* /*data*/) {
        //_exit(-1);
         exit(-1);
    }
};

//this callback is invoked with the new width and height values
void displaySize( void *userData, int width, int height )
{
   EssCtx *ctx = (EssCtx*)userData;
   fprintf(stderr,"essos: display size changed: %dx%d\n", width, height);
   EssContextResizeWindow( ctx, width, height );
}

static EssSettingsListener settingsListener=
{
   displaySize
};

EssCtxHolder* EssCtxHolder::mInstance;


EssCtxHolder::EssCtxHolder()
    : mEssCtx(EssContextCreate())
{
    bool error = false;

    if( !EssContextSetSettingsListener(mEssCtx, mEssCtx, &settingsListener) )
    {
        error = true;
    }
    else if ( !EssContextInit(mEssCtx) )
    {
        error = true;
    }
    else if ( !EssContextSetTerminateListener(mEssCtx, 0, &terminateListener) )
    {
        error = true;
    }

    if ( error )
    {
        const char *detail = EssContextGetLastErrorDetail(mEssCtx);
        fprintf(stderr, "Essos error: '%s'\n", detail);
    }
}

EssCtxHolder::~EssCtxHolder()
{
    stopDispatching();
    EssContextDestroy(mEssCtx);
}

EssCtxHolder* EssCtxHolder::instance()
{
   if( !mInstance )
   {
     mInstance = new EssCtxHolder;
   }
   return mInstance;
}

bool EssCtxHolder::startDispatching()
{
        if ( !EssContextStart(mEssCtx) )
            return false;
    return true;
}

void EssCtxHolder::stopDispatching()
{
        EssContextStop(mEssCtx);
}
