$Id
### imce_watermark Readme

Summary:
  imce_watermark.module is the module for adding watermark to image at imce image uploading.

Requirements:
  1. http://drupal.org/project/imce;
  2. Also if you have installed imagecache_canvasactions, submodule of http://drupal.org/project/imagecache_actions, 
     imagecache presets with watermark action can be used. So you'll get all powerfull of imagecache.

Installation:
  Install as usual, see http://drupal.org/node/70151 for further information.

Upgrading:
  Uninstall previous module and remove it. Branches 1 and 2 are incompatible.

Configuration:
  1. Visit module configuration page at /admin/settings/imce/imce_watermark;
  2. Select "Manual script watermark" for adding watermark manually, it doesn't require any module installed. 
    2.1. Expand "Manual script watermark settings" and upload file of watermark and setup settings for watermarking.
  3. If you want to use imagecache_actions, in imagecache admin page create any preset, one of actions is "Overlay (watermark)"; 
     All settings of watermark will setup here. 
    3.1. Visit imce_watermark setup page and select watermarking preset.
  4. User with role permission: "imce watermark control" can decide adding watermark or not. 
     Otherwise, user cann't control it, and watermark will added anyway.

Contacts:
  Nikit - http://nikiit.ru/en/contact
