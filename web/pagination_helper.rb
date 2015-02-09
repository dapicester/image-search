module PaginationHelper

  def paginate(array, page, size = 10)
    start = (page - 1) * size
    stop  = page * size - 1

    array[start..stop]
  end

  def page
    (params[:page] || 1).to_i
  end

  def pages(array, size = 10)
    (array.count.to_f / 10).ceil
  end

end
