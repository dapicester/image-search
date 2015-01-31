- Multi-threading in feature extraction
- New features
  - Region-of-interest, centered rectangle/ellipse in the image
  - Increase median filter size (color)
  - Experiment blur/sharpen filtering (shape)
  - Apply threshold on returned images distance, so that
    actual #returned <= #requested
- Use msgpack or protobuf for tcp protocol
- Find a better logging library
- Cleanup core classes (Descriptors, Index)
