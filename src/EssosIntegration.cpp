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
