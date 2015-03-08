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

search = (img) ->
  $("#preview img").attr 'src', $(img).attr 'src'
  query = {}
  $.each $("#search").serializeArray(), (i, el) -> query[el.name] = el.value
  query['query_id'] = $(img).data 'id'
  console.log JSON.stringify query
  $.post('/search.json', query).done (data) ->
    results = JSON.parse data
    $.each results, (i, el) ->
      $(".results img:eq(#{i})").attr('src', el[1]).data id: el[0]

$ ->
  $("#category").change -> goto_search current_category()
  show_preview()
  $("#query").change -> show_preview()
  $("#all").click -> goto_all current_category()
  $(".results img").click -> search this
