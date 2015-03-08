module PaginationHelper

  def page
    (params[:page] || 1).to_i
  end

end
