# Updates the preview image.
show_preview = ->
  image = $('#query option:selected').data('image')
  $("#preview > img").attr 'src', image

$ ->
  show_preview()
  $("#query").change -> show_preview()
