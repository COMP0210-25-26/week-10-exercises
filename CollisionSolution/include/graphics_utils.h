#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_main.h>
#include <vector>
#include "circle_collider.hpp"

void RenderScene(std::vector<Body> &bodies, unsigned int x_offset, unsigned int x_buffer, unsigned int height);

void SetupRendering(int rank, unsigned int w, unsigned int h);

void CheckForEvents(bool &keep_going, int n_proc, int rank);

void CheckForTerminationSignal(int n_proc, int rank, bool &keep_going);

void UpdateFrame(Uint64 frame_start, std::vector<Body> &local_bodies, const unsigned int X_OFFSET, const unsigned int X_BUFFER, const unsigned int HEIGHT, int rank, int n_proc, bool &keep_going);
