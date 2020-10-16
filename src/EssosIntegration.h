#ifndef ESSOSINTEGRATION_H
#define ESSOSINTEGRATION_H

#include <essos-app.h>


class EssCtxHolder
{
public:
    static EssCtxHolder* instance();
    EssCtx* getEssCtx() const { return mEssCtx; }
    bool startDispatching();
    void stopDispatching();
private:
    EssCtxHolder();
    ~EssCtxHolder();
    EssCtx *mEssCtx { nullptr };
    static EssCtxHolder* mInstance;
};


#endif /* ESSOSINTEGRATION_H */
