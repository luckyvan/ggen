require "rake/testtask"

task :default => ['test/unit']

Rake::TestTask.new('test/unit') do |test|
	test.libs << "test"
	test.test_files = Dir["test/unit/*_test.rb"]
	test.verbose=true
end
