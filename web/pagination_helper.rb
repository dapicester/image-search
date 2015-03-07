module PaginationHelper

  PAGE_SIZE = 20

  def paginate(array, page, size = PAGE_SIZE)
    start = (page - 1) * size
    stop  = page * size - 1

    array[start..stop]
  end

  def page
    (params[:page] || 1).to_i
  end

  def pages(array, size = PAGE_SIZE)
    (array.count.to_f / size).ceil
  end

end
