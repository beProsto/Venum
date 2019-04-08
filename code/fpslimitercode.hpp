#ifndef FPSLIMITERCODE_HPP_INCLUDED
#define FPSLIMITERCODE_HPP_INCLUDED

ven::FPSLimiter::FPSLimiter( unsigned int _fpslimit )
{
    this->SetFrameRateLimit( _fpslimit );
}

ven::FPSLimiter::~FPSLimiter(  )
{

}

void ven::FPSLimiter::SetFrameRateLimit( unsigned int _fpslimit )
{
    this->fpslimit = _fpslimit;
}

void ven::FPSLimiter::UpdateStart(  )
{
    this->cycle = SDL_GetTicks(  );
}

void ven::FPSLimiter::UpdateEnd(  )
{
    unsigned int now;
    unsigned int difference;
    float delay;

    now = SDL_GetTicks(  );
    difference = now - this->cycle;
    delay = ( 1000.0f / this->fpslimit ) - difference;
    if( delay > 0 )
    {
        SDL_Delay( delay );
    }
}


#endif // FPSLIMITERCODE_HPP_INCLUDED
