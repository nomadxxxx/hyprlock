#pragma once

#include "IWidget.hpp"
#include "../../helpers/Color.hpp"
#include "../../helpers/Math.hpp"
#include "../../core/Timer.hpp"
#include "../Framebuffer.hpp"
#include "../AsyncResourceGatherer.hpp"
#include <hyprutils/math/Misc.hpp>
#include <string>
#include <unordered_map>
#include <any>
#include <chrono>
#include <filesystem>

struct SPreloadedAsset;
class COutput;

struct SFade {
    std::chrono::system_clock::time_point start;
    float                                 a              = 0;
    std::shared_ptr<CTimer>               crossFadeTimer = nullptr;
};

class CBackground : public IWidget {
  public:
    CBackground() = default;
    ~CBackground();

    void         registerSelf(const SP<CBackground>& self);

    virtual void configure(const std::unordered_map<std::string, std::any>& props, const SP<COutput>& pOutput) override;
    virtual bool draw(const SRenderData& data) override;
    virtual std::string type() const override;
    virtual bool isVideoBackground() const; // Method for video background check

    void         reset(); // Unload assets, remove timers, etc.

    void         renderRect(CHyprColor color);

    void         onReloadTimerUpdate();
    void         onCrossFadeTimerUpdate();
    void         plantReloadTimer();
    void         startCrossFadeOrUpdateRender();

    // Members for video background support
    bool         m_bIsVideoBackground = false; // Renamed to avoid conflict
    std::string  videoPath;
    std::string  monitor; // Store monitor name for mpvpaper
    std::string  fallbackPath; // Added for fallback image if video fails

  private:
    WP<CBackground> m_self;

    // if needed
    CFramebuffer                            blurredFB;

    int                                     blurSize          = 10;
    int                                     blurPasses        = 3;
    float                                   noise             = 0.0117;
    float                                   contrast          = 0.8916;
    float                                   brightness        = 0.8172;
    float                                   vibrancy          = 0.1696;
    float                                   vibrancy_darkness = 0.0;
    Vector2D                                viewport;
    std::string                             path = "";

    std::string                             outputPort;
    Hyprutils::Math::eTransform             transform;

    std::string                             resourceID;
    std::string                             pendingResourceID;

    float                                   crossFadeTime = -1.0;

    CHyprColor                              color;
    SPreloadedAsset*                        asset        = nullptr;
    bool                                    isScreenshot = false;
    SPreloadedAsset*                        pendingAsset = nullptr;
    bool                                    firstRender  = true;

    UP<SFade>                               fade;

    int                                     reloadTime = -1;
    std::string                             reloadCommand;
    CAsyncResourceGatherer::SPreloadRequest request;
    std::shared_ptr<CTimer>                 reloadTimer;
    std::filesystem::file_time_type         modificationTime;
};