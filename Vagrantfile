# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure(2) do |config|
  config.vm.box = "chef/debian-7.6"
  config.vm.provision :ansible do |ansible|
    ansible.playbook = 'provision/vagrant.yml'
  end
end
