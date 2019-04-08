#ifndef TIMERCODE_HPP_INCLUDED
#define TIMERCODE_HPP_INCLUDED

ven::Timer::Timer(  )
: timeStart( 0 ), timeElapsed( 0 ), deltaTime( 0 ), timeScale( 0 )
{  }

ven::Timer::~Timer(  )
{  }

void ven::Timer::Update(  )
{
    this->timeElapsed = SDL_GetTicks(  ) - this->timeStart;
    this->timeStart = SDL_GetTicks(  );
    this->deltaTime = this->timeElapsed * 0.001f;
}

#endif // TIMERCODE_HPP_INCLUDED
