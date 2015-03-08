# Updates the preview image.
show_preview = ->
  image = $('#query option:selected').data('image')
  $("#preview > img").attr 'src', image

current_category = ->
  $("#category option:selected").val()

goto_all = (category) ->
  window.location.href = "/all?category=#{category}"

goto_search = (category) ->
  window.location.href = "/search?category=#{category}"

$ ->
  $("#category").change -> goto_search current_category()
  show_preview()
  $("#query").change -> show_preview()
  $("#all").click -> goto_all current_category()
