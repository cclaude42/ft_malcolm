# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|
  config.vm.define :alpha do |alpha|
    alpha.vm.box = "hashicorp/bionic64"
    alpha.vm.network :private_network, ip: "192.168.56.10"
    alpha.vm.hostname = "alpha"
    alpha.vm.synced_folder '.', '/vagrant', disabled: true
  end

  config.vm.define :beta do |beta|
    beta.vm.box = "hashicorp/bionic64"
    beta.vm.network :private_network, ip: "192.168.56.11"
    beta.vm.hostname = "beta"
    beta.vm.synced_folder '.', '/vagrant', disabled: true
  end

  config.vm.define :gamma do |gamma|
    gamma.vm.box = "hashicorp/bionic64"
    gamma.vm.network :private_network, ip: "192.168.56.12"
    gamma.vm.hostname = "gamma"
    gamma.vm.synced_folder '.', '/vagrant', disabled: true
  end
end
