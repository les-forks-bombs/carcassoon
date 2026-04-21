#include "text.h"

text_object_t *init_text_object(SDL_Renderer *renderer, char *font_path, float font_size, const char *content, SDL_Color color) {
    text_object_t *to = SDL_malloc(sizeof(text_object_t));
    if (!to) return NULL;
    to->font = TTF_OpenFont(font_path, font_size);
    if (!to->font) {
        SDL_free(to);
        return NULL;
    }
    to->font_size=font_size;
    to->color=color;
    to->content=SDL_strdup(content);
    to->texture=NULL;
    update_text_object(to,renderer);
    return to;
}

void update_text_object(text_object_t *to, SDL_Renderer *renderer) {
    if (!to || !to->font) return;

    SDL_Surface *surf = TTF_RenderText_Blended(to->font, to->content, 0, to->color);
    if (!surf) return;

    if (to->texture) {
        SDL_DestroyTexture(to->texture);
        
    }

    to->texture = SDL_CreateTextureFromSurface(renderer, surf);

    if (to->texture) {
        to->w = (float)surf->w;
        to->h = (float)surf->h;
    }

    SDL_DestroySurface(surf);
    
    return;
}

void destroy_text_object(text_object_t *to) {
    if (!to) return;
    if (to->texture) SDL_DestroyTexture(to->texture);
    if (to->font) TTF_CloseFont(to->font);
    if (to->content) SDL_free(to->content);
    SDL_free(to);
}
