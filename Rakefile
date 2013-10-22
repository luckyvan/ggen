require "rake/testtask"
require "rubygems/package_task"

task :default => ['test/unit']

Rake::TestTask.new('test/unit') do |test|
	test.libs << "test"
	test.test_files = Dir["test/unit/*_test.rb"]
	test.verbose=true
end

desc "Install ggen as a softlink under /usr/bin"
task :install do |install|
  dst = File.dirname(__FILE__) + '/bin/ggen'
  puts "Creating softlink /usr/bin/ggen to #{dst}"
  FileUtils.ln_s dst, '/usr/bin/ggen', :force => true
end
