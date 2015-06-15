
TeeFile = "plots/rfc.raw"
ImportantLines = (7..20).to_a
ImportantColumns = (1..27).map {|i| i*3 }

filelines = File.readlines(TeeFile)
important_lines = ImportantLines.map do |i|
  filelines[i]
end

data = {}
cdata = {}

important_lines.each_with_index do |line, j|
  cols = line.split
  ImportantColumns.each_with_index do |v, i|
    data[i] ||= []
    data[i] << cols[v]
    cdata[j] ||= []
    cdata[j] << cols[v]
  end
end

def average(xs)
  sum = xs.map(&:to_f).reduce(&:+)
  (sum / xs.count).round(2)
end

data.each do |k,v|
  vals = v.map {|x| x[0..-2]}
  puts "#{k} #{vals.join(" ")} #{average(vals)}"
end

puts
puts

cdata.each do |k,v|
  vals = v.map {|x| x[0..-2]}
  puts "#{k} #{vals.join(" ")} #{average(vals)}"
end

# puts; puts; puts important_lines
