enum class PostProcessMode {
    None,
    Invert,
    Grayscale,
    EdgeDetection,
    Pixelization,
};

PostProcessMode currentMode = PostProcessMode::None;
