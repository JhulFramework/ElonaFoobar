#include "image.hpp"
#include "application.hpp"
#include "renderer.hpp"


using namespace elona::snail;


namespace
{


void render(
    ::SDL_Texture* texture,
    ::SDL_Renderer* renderer,
    BlendMode blend_mode,
    int src_x,
    int src_y,
    int src_width,
    int src_height,
    int dst_x,
    int dst_y,
    int dst_width,
    int dst_height)
{
    switch (blend_mode)
    {
    case BlendMode::none:
        detail::enforce_sdl(
            ::SDL_SetTextureBlendMode(texture, ::SDL_BLENDMODE_NONE));
        break;
    case BlendMode::blend:
        detail::enforce_sdl(
            ::SDL_SetTextureBlendMode(texture, ::SDL_BLENDMODE_BLEND));
        break;
    case BlendMode::add:
        detail::enforce_sdl(
            ::SDL_SetTextureBlendMode(texture, ::SDL_BLENDMODE_ADD));
        break;
    }

    auto src = ::SDL_Rect{src_x, src_y, src_width, src_height};
    auto dst = ::SDL_Rect{dst_x, dst_y, dst_width, dst_height};
    detail::enforce_sdl(::SDL_RenderCopy(renderer, texture, &src, &dst));
}



} // namespace



namespace elona
{
namespace snail
{



BasicImage::BasicImage(
    const fs::path& filepath,
    const optional<Color>& keycolor)
{
    auto surface = detail::enforce_img(::IMG_Load(filepath.string().c_str()));

    if (keycolor)
    {
        detail::enforce_sdl(::SDL_SetColorKey(
            surface,
            true,
            ::SDL_MapRGB(
                surface->format, keycolor->r, keycolor->g, keycolor->b)));
    }

    _ptr.reset(
        detail::enforce_sdl(::SDL_CreateTextureFromSurface(
            Application::instance().get_renderer().ptr(), surface)),
        ::SDL_DestroyTexture);
    _width = surface->w;
    _height = surface->h;

    ::SDL_FreeSurface(surface);
}



BasicImage::BasicImage(::SDL_Texture* ptr)
{
    _ptr.reset(ptr, ::SDL_DestroyTexture);
}



void BasicImage::_render(
    ::SDL_Renderer* renderer,
    BlendMode blend_mode,
    int src_x,
    int src_y,
    int src_width,
    int src_height,
    int dst_x,
    int dst_y,
    int dst_width,
    int dst_height)
{
    render(
        ptr(),
        renderer,
        blend_mode,
        src_x,
        src_y,
        src_width,
        src_height,
        dst_x,
        dst_y,
        dst_width,
        dst_height);
}



FrameImage::FrameImage(
    BasicImage& source,
    int offset_x,
    int offset_y,
    int width,
    int height)
    : _offset_x(offset_x)
    , _offset_y(offset_y)
{
    _width = width;
    _height = height;
    _ptr.reset(source.ptr(), ::SDL_DestroyTexture);
}



void FrameImage::_render(
    ::SDL_Renderer* renderer,
    BlendMode blend_mode,
    int src_x,
    int src_y,
    int src_width,
    int src_height,
    int dst_x,
    int dst_y,
    int dst_width,
    int dst_height)
{
    render(
        ptr(),
        renderer,
        blend_mode,
        src_x + _offset_x,
        src_y + _offset_y,
        src_width,
        src_height,
        dst_x,
        dst_y,
        dst_width,
        dst_height);
}


} // namespace snail
} // namespace elona
