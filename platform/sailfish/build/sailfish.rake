#------------------------------------------------------------------------
# (The MIT License)
# 
# Copyright (c) 2008-2011 Rhomobile, Inc.
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# 
# http://rhomobile.com
#------------------------------------------------------------------------

require File.dirname(__FILE__) + '/../../../lib/build/BuildConfig'

require 'pathname'
require 'tempfile'

include FileUtils

require 'erb'
require 'net/ssh'
#require 'net/scp'

class QtProjectGenerator
  attr_accessor :rhoRoot
  attr_accessor :extRoot
  attr_accessor :nameApp
  attr_accessor :versionApp
  attr_accessor :buildMode

  def render_profile(erbPath)
    tpl = File.read erbPath
    erb = ERB.new tpl
    erb.result binding
  end
end

class ScriptGenerator
  attr_accessor :merPort
  attr_accessor :merPkey
  attr_accessor :projectPath
  attr_accessor :merSdkTools
  attr_accessor :merSharedHome
  attr_accessor :merSharedSrc
  attr_accessor :merShTgtName
  attr_accessor :merTgtName
  attr_accessor :merUserName
  attr_accessor :merDevName

  def render_script(erbPath)
    tpl = File.read erbPath
    erb = ERB.new tpl
    erb.result binding
  end
end

class BuildScriptGenerator < ScriptGenerator
  attr_accessor :qmakePath
  attr_accessor :proPath
  attr_accessor :buildMode
  attr_accessor :qmlMode
  attr_accessor :makePath
  attr_accessor :rpmPath
  attr_accessor :rpmvalPath
  attr_accessor :deployPath
end

def isWindows?
  if /cygwin|mswin|mingw|bccwin|wince|emx/ =~ RUBY_PLATFORM
      return true
  else
      return false
  end
end

namespace "config" do
  task :set_sailfish_platform do
    $current_platform = "sailfish"
  end

  task :sailfish => :set_sailfish_platform do
    print_timestamp('config:sailfish START')
    Rake::Task["config:common"].invoke()
    Rake::Task["config:qt"].invoke()

    if !$app_config.nil? && !$app_config["sailfish"].nil?
      $connf_build = "Release"
      if !$app_config["sailfish"]["configuration"].nil?
        $connf_build = $app_config["sailfish"]["configuration"]
      end
      
      $target_arch = "arm"
      if !$app_config["sailfish"]["target"].nil?
        $target_arch = $app_config["sailfish"]["target"]
      end

      puts "Target arch: " + $target_arch
      puts "Target configuration: " + $connf_build
    else
      raise "Sailfish section is not found!"
    end

    if !$app_config["name"].nil?    
      $appname = $app_config["name"]
      $final_name_app = "harbour-" + "#{$appname.downcase}"
    else
      raise "Name application is not found in build.yaml!"
    end

    if !$app_config["version"].nil?
      $version_app = $app_config["version"]
    else
      $version_app = "1.0"
    end

    require 'nokogiri'
    current_targets = File.join($qtdir, "mersdk", "targets", "targets.xml")
    current_targets = current_targets.gsub("\\", "/")

    doc = File.open(current_targets) { |f| Nokogiri::XML(f) }
    $current_build_sdk_dir = ""
    doc.xpath("//target").each do |t|
      target = t.attr("name")
      if target.include? $target_arch
        $current_target = target
        $current_build_sdk_dir = File.join($qtdir, "settings", "SailfishOS-SDK", 
        "mer-sdk-tools", "Sailfish OS Build Engine", target)
        $current_build_sdk_dir = $current_build_sdk_dir.gsub("\\", "/")
        break
      end
    end

    $qtdir = ENV['QTDIR']

    if $current_build_sdk_dir == ""
      raise "Build arch sdk not found!"
    end

    puts "Current building sdk: " + $current_build_sdk_dir
    $excludelib = ['**/builtinME.rb', '**/ServeME.rb', '**/dateME.rb', '**/rationalME.rb']
    $rho_path =  File.join($app_path, "data", "rho")
    $srcdir = $rho_path
    $project_path = File.join($app_path, "project", "qt")
    $app_project_path = File.join($project_path, $final_name_app)

    $target_path = File.join($app_path, "bin", "target", "sailfish")
    mkdir_p $target_path

    if !$app_config.nil? && !$app_config["sailfish"].nil?
      $host_name = "192.168.2.15"
      $user_name = "nemo"

      if !$app_config["sailfish"]["device"].nil? && !$app_config["sailfish"]["device"]["type"].nil?
        $dev_type = $app_config["sailfish"]["device"]["type"]
      else
        $dev_type = "vbox"
      end

      if !$app_config["sailfish"]["device"].nil? && !$app_config["sailfish"]["device"]["host"].nil?
        $host_name = $app_config["sailfish"]["device"]["host"]
      end

      if !$app_config["sailfish"]["device"].nil? && !$app_config["sailfish"]["device"]["user"].nil?
        $user_name = $app_config["sailfish"]["device"]["user"]
      end
      
      if !$app_config["sailfish"]["device"].nil? && !$app_config["sailfish"]["device"]["device_name"].nil?
        $dev_name = $app_config["sailfish"]["device"]["device_name"]
      elsif $dev_type == "real"
        raise "Please set dev_name for real device in device section!"
      elsif $dev_type == "vbox"
        $dev_name = "Sailfish OS Emulator"
      end

    end

    #TODO: windows path way
    $virtualbox_path = ENV['VBOX_MSI_INSTALL_PATH']
    Rake::Task["build:sailfish:startvm"].invoke()
  end

end

def exec_ssh_command(session, cmd)
  #session.scp.upload!($target_rpm, "/home/#{$user_name}/RPMS")
  #session.open_channel do |channel|
    #channel.on_request "exit-status" do |channel, data|
      #$exit_status = data.read_long
    #end
    #puts "devel-su rpm -Uvh /home/#{$user_name}/RPMS/#{File.basename $target_rpm}"

    #channel.exec("rpm -Uvh /home/#{$user_name}/RPMS/#{File.basename $target_rpm}") do |devel_channel, success|

      #devel_channel.on_data do |ch, data|
        #puts "data1"
      #end

      #devel_channel.on_extended_data do |ch, type, data|
        #puts "error1"
      #end

      #if !success
        #raise "Open interactive mode failed!"
      #else
        #devel_channel.send_data("#{$pwd_host}\n")
      #end
      #stdout << stream
      #puts stdout
      #devel_channel.wait
    #end
    #channel.wait
  #end
end

namespace "run" do
  task :sailfish => ["config:sailfish"] do
    if !$app_config["sailfish"]["device"].nil? && !$app_config["sailfish"]["device"]["key"].nil?
      $ssh_key = $app_config["sailfish"]["device"]["key"]
    end
  
    if !$app_config["sailfish"]["device"].nil? && !$app_config["sailfish"]["device"]["password"].nil?
      $pwd_host = $app_config["sailfish"]["device"]["password"]
    else
      raise "Key or password for running app not found, set it!"
    end

    session_ssh = nil  
    if !$app_config["sailfish"]["device"].nil? && !$app_config["sailfish"]["device"]["key"].nil?    
      Net::SSH.start($host_name, $user_name, :host_key => "ssh-rsa", :keys => [ $ssh_key ]) do |session| 
        exec_ssh_command(session)        
      end  
    else    
      Net::SSH.start($host_name, $user_name, $pwd_host) do |session|       
        exec_ssh_command(session)    
      end
    end

  end
end

namespace "device" do
  namespace "sailfish" do
    task :production => "build:sailfish:rhobundle" do
      Rake::Task["build:sailfish:rhodes"].invoke()
    end

    task :install => ["config:sailfish"] do
      Rake::Task["build:sailfish:startvm"].invoke()
      Rake::Task["build:sailfish:deploy"].invoke()
    end

  end
end

namespace "clean" do
  task :sailfish => "clean:sailfish:files"
  namespace "sailfish" do
    task :files => "config:sailfish" do
      clean_path = File.join($app_path, "project", "qt", "clean.cmd")
      system(clean_path)
      if File.exists?($project_path)
        Dir.glob(File.join($project_path, "*.*")) { |f| rm f, :force => true }
        Dir.entries(File.join($project_path)).each do |d|
          rm_rf File.join($project_path, d)
        end
      end
      rm_rf $srcdir if File.exists?($srcdir)
      #rm_rf $targetdir
      #rm_rf $app_builddir
      #rm_rf $tmpdir
    end
    task :build => "config:sailfish" do
      clean_path = File.join($app_path, "project", "qt", "clean.cmd")
      system(clean_path)
    end
  end
end

def add_extension_to_main_project(ext)
   main_project = File.join($project_path, $final_name_app, "#{$final_name_app}.pro")

   f = File.open(main_project, "r")
   text = ''
   f.each_line do |line|
     if line.include?('SUBDIRS') and !line.include?("$$PWD/../#{ext.downcase}")
      #before continue /r/n/ symbols and \\
      line.insert(-3, " $$PWD/../#{ext.downcase}")
      #line = line + "$$PWD/../#{ext.downcase}/r/n"
     end
     text = text + line
   end
   f.close

   f = File.open(main_project, "w") {|file| file << text }
end

def add_extension_to_rhodes_project(ext)

  rhodes_project = File.join($project_path, "rhodes", "rhodes.pro")

  f = File.open(rhodes_project, "r")
  text = ''
  ext_exists = false

  f.each_line do |line|
    if line.include?("$$PWD/../extensions -l#{ext.downcase}")
      ext_exists = true
    end
    text = text + line
  end
  f.close

  if !ext_exists
    text = text + "unix:!macx: LIBS += -L$$PWD/../extensions -l#{ext.downcase} \n"
    #text = text + "unix:!macx: PRE_TARGETDEPS += $$PWD/../extensions/lib#{ext.downcase}.a \n"
  end

  f = File.open(rhodes_project, "w") {|file| file << text }
  return 
end

def vm_is_started?
  require 'open3'

  output = ""
  cmd = "\"" + File.join($virtualbox_path, "VBoxManage.exe") + "\"" + " list runningvms"
  Open3.popen3(cmd) do |stdin, stdout, stderr, wait_thr|
    if wait_thr.value != 0 
        raise "Shell return error: #{wait_thr.value.to_s}\n STDOUT: #{stdout.read}" 
    end
    output = stdout.read
  end

  return output.include?("Sailfish OS Build Engine")
end

namespace "build"  do
  namespace "sailfish" do

    task :startvm => "config:sailfish" do
      if $virtualbox_path.empty? 
        raise "Please, set VirtualBox variable environment..."
      end

      if !vm_is_started?
        system("\"" + File.join($virtualbox_path, "VBoxManage.exe") + "\"" + " startvm \"Sailfish OS Build Engine\" --type headless") 
        puts "Waiting 40 seconds vm..."
        sleep 40.0
      end 
    end

    task :stopvm => "config:sailfish" do
      if $virtualbox_path.empty? 
        raise "Please, set VirtualBox variable environment..."
      end
      system("\"" + File.join($virtualbox_path, "VBoxManage.exe") + "\"" + " controlvm \"Sailfish OS Build Engine\" poweroff")
    end
    
    task :rhobundle => ["project:sailfish:qt"] do
      print_timestamp('build:sailfish:rhobundle START')
      
      $data_path = File.join($app_project_path, "data")
      unless File.directory?($data_path)
        mkdir $data_path
      end

      $rho_path =  File.join($data_path, "rho")
      $srcdir = $rho_path

      rm_rf File.join($rho_path)
      unless File.directory?($rho_path)
        mkdir $rho_path
      end
      Rake::Task["build:bundle:noxruby"].invoke

      if !$skip_build_extensions
        Rake::Task["build:sailfish:extensions"].execute
      end
      Jake.build_file_map( File.join($srcdir, "apps"), "rhofilelist.txt" )
    end

    task :deploy => ['config:sailfish'] do
      deploy_path = File.join($app_path, "project", "qt", "deploy.cmd")
      system(deploy_path)
    end

    task :rhodes => ["project:sailfish:qt"] do
      print_timestamp('build:sailfish:rhodes START')
      build_path = File.join($app_path, "project", "qt", "build.cmd")
      rpm_path = File.join($app_path, "project", "qt", "rpm.cmd")
      system(build_path)
      system(rpm_path)

      $target_rpm = ""
      Dir[File.join($project_path, $final_name_app, "RPMS/**/*")].each do |file_name|
        if file_name.include?("#{$final_name_app}-#{$version_app}")
          $target_rpm = file_name
          puts "target rpm: " + $target_rpm
        end

      #if !File.exists?(target_rpm) raise "Target rpm not found!"
      rpmval_path = File.join($project_path, "rpmvalidation.cmd")
      system("\"#{rpmval_path}\" #{$target_rpm}")
      cp_r $target_rpm, $target_path

      end

    end

    task :extensions => ['project:sailfish:qt'] do
      puts "$app_extensions_list : #{$app_extensions_list}"

      ext_link = "unix:!macx: PRE_TARGETDEPS += $$PWD/../extensions"
      $app_extensions_list.each do |ext, ext_path |
        next unless (ext_path)
        print_timestamp('process extension "' + ext + '" START')

        extpath = File.join( ext_path, 'ext')
        ext_config_path = File.join( ext_path, "ext.yml")
        ext_config = nil
          
        if File.exist? ext_config_path
          ext_config = Jake.config(File.open(ext_config_path))
        end

        erb_project_path = ext_config[$current_platform]["proerb"] if ( ext_config && ext_config[$current_platform] && ext_config[$current_platform]["proerb"])
        next unless (erb_project_path)

        erb_project_path = File.join ext_path, erb_project_path
        puts "Path to erb project: " + erb_project_path

        generator = QtProjectGenerator.new
        generator.rhoRoot = $rhodes_path_build_engine   
        generator.extRoot = ext_path.gsub(File.expand_path('~'), "/home/mersdk/share")   
        generator.nameApp = $final_name_app 
        generator.buildMode = $connf_build
        
        mkdir_p File.join($project_path, ext.downcase)

        File.open(File.join($project_path, ext.downcase, "#{ext.downcase}.pro"), 'w' ) { |f| f.write generator.render_profile( erb_project_path ) }
        add_extension_to_main_project(ext)
        if ext.downcase == "coreapi"
          next
        end
        add_extension_to_rhodes_project(ext)
      end
      add_extension_to_rhodes_project("coreapi")
    end

  end
end

namespace 'project' do
  namespace 'sailfish' do
    task :qt => ['config:sailfish'] do
      $rhodes_path = File.absolute_path '.'
      $rhodes_path_build_engine = $rhodes_path.gsub(File.expand_path('~'), "/home/mersdk/share")
      
      curl_erb_path = $rhodes_path + "/platform/sailfish/build/curl.pro.erb"
      rubylib_erb_path = $rhodes_path + "/platform/sailfish/build/rubylib.pro.erb"
      rholib_erb_path = $rhodes_path + "/platform/sailfish/build/rholib.pro.erb"
      sqlite3_erb_path = $rhodes_path + "/platform/sailfish/build/sqlite3.pro.erb"
      syncengine_erb_path = $rhodes_path + "/platform/sailfish/build/syncengine.pro.erb"
      rhodes_erb_path = $rhodes_path + "/platform/sailfish/build/rhodes.pro.erb"
      app_erb_path = $rhodes_path + "/platform/sailfish/build/harbour-SailfishRhodes.pro.erb"

      desktop_erb_path = $rhodes_path + "/platform/sailfish/build/rpm/harbour-SailfishRhodes.desktop.erb"
      yaml_erb_path = $rhodes_path    + "/platform/sailfish/build/rpm/harbour-SailfishRhodes.yaml.erb"

      build_erb_path = $rhodes_path + "/platform/sailfish/build/rho_build.cmd.erb"
      clean_erb_path = $rhodes_path + "/platform/sailfish/build/rho_clean.cmd.erb"
      rpm_erb_path = $rhodes_path + "/platform/sailfish/build/rho_rpm.cmd.erb"
      rpmval_erb_path = $rhodes_path + "/platform/sailfish/build/rho_rpmvalidation.cmd.erb"
      deploy_erb_path = $rhodes_path + "/platform/sailfish/build/rho_deploy.cmd.erb"

      mkdir_p $project_path
      mkdir_p File.join($project_path, "curl")
      mkdir_p File.join($project_path, "rubylib")
      mkdir_p File.join($project_path, "rholib")
      mkdir_p File.join($project_path, "sqlite3")
      mkdir_p File.join($project_path, "syncengine")
      mkdir_p File.join($project_path, "rhodes")
      mkdir_p File.join($project_path, $final_name_app)
      mkdir_p File.join($project_path, $final_name_app, "rpm")

      if !File.exists?(File.join($project_path, $final_name_app, "qml")) 
        cp_r File.join($rhodes_path, "platform/shared/qt/sailfish/qml"), File.join($project_path, $final_name_app)
        File.rename(
          File.join($project_path, $final_name_app, "qml", "harbour-sailfishrhodes.qml"), 
          File.join($project_path, $final_name_app, "qml", "#{$final_name_app}.qml"))
      end

      if !File.exists?(File.join($project_path, $final_name_app, "icons")) 
        cp_r File.join($rhodes_path, "platform/shared/qt/sailfish/icons"), File.join($project_path, $final_name_app)
        Dir[File.join($project_path, $final_name_app, "icons/**/*")].each do |file_name|
          File.rename(file_name, File.join(File.dirname(file_name), "#{$final_name_app}.png")) if File.file? file_name
        end
      end

      generator = QtProjectGenerator.new
      generator.rhoRoot = $rhodes_path_build_engine
      generator.nameApp = $final_name_app
      generator.versionApp = $version_app     
      generator.buildMode = $connf_build
      
      File.open(File.join($project_path, "curl", "curl.pro"), 'w' ) { |f| f.write generator.render_profile( curl_erb_path ) }
      File.open(File.join($project_path, "rubylib", "rubylib.pro"), 'w' ) { |f| f.write generator.render_profile( rubylib_erb_path ) }
      File.open(File.join($project_path, "rholib", "rholib.pro"), 'w' ) { |f| f.write generator.render_profile( rholib_erb_path ) }
      File.open(File.join($project_path, "sqlite3", "sqlite3.pro"), 'w' ) { |f| f.write generator.render_profile( sqlite3_erb_path ) }
      File.open(File.join($project_path, "syncengine", "syncengine.pro"), 'w' ) { |f| f.write generator.render_profile( syncengine_erb_path ) }
      File.open(File.join($project_path, $final_name_app, "#{$final_name_app}.desktop"), 'w' ) { |f| f.write generator.render_profile( desktop_erb_path ) }
      File.open(File.join($project_path, $final_name_app, "rpm", "#{$final_name_app}.yaml"), 'w' ) { |f| f.write generator.render_profile( yaml_erb_path ) }
      
      rho_name_project = File.join($project_path, "rhodes", "rhodes.pro")
      if !File.exists?(rho_name_project)
        File.open(rho_name_project, 'w' ) { |f| f.write generator.render_profile( rhodes_erb_path ) }
      end
      
      app_name_project = File.join($project_path, $final_name_app, "#{$final_name_app}.pro")
      if !File.exists?(app_name_project)
        File.open(app_name_project, 'w' ) { |f| f.write generator.render_profile( app_erb_path ) }
      end
      
      build_script_generator = BuildScriptGenerator.new
      build_script_generator.merPort = 2222
      build_script_generator.merPkey = File.join $qtdir, "vmshare/ssh/private_keys/engine/mersdk"
      build_script_generator.projectPath = File.join($project_path, $final_name_app)
      build_script_generator.merSdkTools = $current_build_sdk_dir.gsub("/", "\\")
      build_script_generator.merSharedHome = File.expand_path('~')
      build_script_generator.merSharedSrc = File.expand_path('~')
      build_script_generator.merShTgtName = File.join($qtdir, "mersdk", "targets")
      build_script_generator.merTgtName = $current_target
      build_script_generator.merUserName = "mersdk"
      build_script_generator.merDevName = $dev_name

      #TODO: windows paths way - temporary
      build_script_generator.qmakePath = File.join($current_build_sdk_dir, "qmake.cmd").gsub("/", "\\")
      build_script_generator.proPath = File.join($project_path, $final_name_app, "#{$final_name_app}.pro").gsub("/", "\\")
      build_script_generator.buildMode = $connf_build.downcase
      build_script_generator.qmlMode = "qml_#{$connf_build.downcase}" 
      build_script_generator.makePath = File.join($current_build_sdk_dir, "make.cmd").gsub("/", "\\")
      build_script_generator.rpmPath =  File.join($current_build_sdk_dir, "rpm.cmd").gsub("/", "\\")
      build_script_generator.rpmvalPath =  File.join($current_build_sdk_dir, "rpmvalidation.cmd").gsub("/", "\\")
      build_script_generator.deployPath =  File.join($current_build_sdk_dir, "deploy.cmd").gsub("/", "\\")

      File.open(File.join($project_path, "build.cmd"), 'w' ) { |f| f.write build_script_generator.render_script( build_erb_path ) }
      File.open(File.join($project_path, "clean.cmd"), 'w' ) { |f| f.write build_script_generator.render_script( clean_erb_path ) }
      File.open(File.join($project_path, "rpm.cmd"), 'w' ) { |f| f.write build_script_generator.render_script( rpm_erb_path ) }
      File.open(File.join($project_path, "rpmvalidation.cmd"), 'w' ) { |f| f.write build_script_generator.render_script( rpmval_erb_path ) }
      File.open(File.join($project_path, "deploy.cmd"), 'w' ) { |f| f.write build_script_generator.render_script( deploy_erb_path ) }
    end
  end
end

#MER_SSH_PORT, 2222
#MER_SSH_PRIVATE_KEY, C:/SailfishOS/vmshare/ssh/private_keys/engine/mersdk
#MER_SSH_PROJECT_PATH, C:/Users/n0men/Desktop/tau/testapp/project/qt/curl
#MER_SSH_SDK_TOOLS, C:\SailfishOS\settings\SailfishOS-SDK\mer-sdk-tools\Sailfish OS Build Engine\SailfishOS-2.1.3.7-armv7hl
#MER_SSH_SHARED_HOME, C:/Users/n0men
#MER_SSH_SHARED_SRC, C:/Users/n0men/
#MER_SSH_SHARED_TARGET, C:/SailfishOS/mersdk/targets
#MER_SSH_TARGET_NAME, SailfishOS-2.1.3.7-armv7hl
#MER_SSH_USERNAME, mersdk

#MER_SSH_USERNAME, mersdk
#MER_SSH_TARGET_NAME, SailfishOS-2.1.3.7-i486
#MER_SSH_SHARED_TARGET, C:/SailfishOS/mersdk/targets
#MER_SSH_SHARED_SRC, C:/Users/n0men/
#MER_SSH_SHARED_HOME, C:/Users/n0men
#MER_SSH_SDK_TOOLS, C:\SailfishOS\settings\SailfishOS-SDK\mer-sdk-tools\Sailfish OS Build Engine\SailfishOS-2.1.3.7-i486
#MER_SSH_PROJECT_PATH, C:/Users/n0men/Desktop/tau/kitchensinkRuby/project/qt/harbour-kitchensinkRuby
#MER_SSH_PRIVATE_KEY, C:/SailfishOS/vmshare/ssh/private_keys/engine/mersdk
#MER_SSH_PORT, 2222
#MER_SSH_DEVICE_NAME, INOI R7 (ARM)
