#ifndef FPSLIMITERCODE_HPP_INCLUDED
#define FPSLIMITERCODE_HPP_INCLUDED

ven::FPSLimiter::FPSLimiter( unsigned int _fpsinvertal )
{
    this->SetFrameRateLimitInvertal( _fpsinvertal );
}

ven::FPSLimiter::~FPSLimiter(  )
{

}

void ven::FPSLimiter::SetFrameRateLimitInvertal( unsigned int _fpsinvertal )
{
    this->fpsinvertal = _fpsinvertal;
}

void ven::FPSLimiter::Start(  )
{
    this->cycle = SDL_GetTicks(  ) + this->fpsinvertal;
}

void ven::FPSLimiter::Update(  )
{
    SDL_Delay( this->TimeLeft(  ) );
    this->cycle += this->fpsinvertal;
}

unsigned int ven::FPSLimiter::TimeLeft(  )
{
    unsigned int now;
    now = SDL_GetTicks(  );

    if( this->cycle <= now )
    {
        return 0;
    }
    else
    {
        return this->cycle - now;
    }
}


#endif // FPSLIMITERCODE_HPP_INCLUDED
