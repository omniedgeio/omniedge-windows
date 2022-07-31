# Omniedge-windows

Windows desktop application for Omniedge. 

>Bring the intranet on the internet

[Download Windows](https://omniedge.io/install/download/0.2.3/omniedge-setup-0.2.3.exe)

<!-- [![Build Status]() -->
[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](http://www.gnu.org/licenses/gpl-3.0)

[🤝 Website](https://omniedge.io)
[💬 Twitter](https://twitter.com/omniedgeio)
[😇 Discord](https://discord.gg/d4faRPYj)

A cross-platform private network tool for developers.

![OmniEdge-Windows](https://user-images.githubusercontent.com/93888/171177275-6ec8e7f6-3ea3-4163-8e1a-ef33c5ed0b9f.png)



## Install

### Install OmniEdge Cli

```bash
curl https://omniedge.io/install/omniedge-install.sh | bash
```

### Install OmniEdge Gui Client

-   [Android: OmniEdge.apk](https://omniedge.io/install/download/0.2.2/omniedge-release-v0.2.2.apk)
-   [macOS cli](https://omniedge.io/install/download/0.2.3/omniedgecli-macos-latest.zip)
-   [Windows](https://omniedge.io/install/download/0.2.3/omniedge-setup-0.2.3.exe)
-   [Linux Cli](https://github.com/omniedgeio/app-release/releases/tag/v0.2.3)
-   [iOS & M1 Mac on App Store](https://apps.apple.com/us/app/omniedgenew/id1603005893)
-   [Synology](https://omniedge.io/download/synology)
-   [Raspberry Pi, ARM, Nvidia Jetson](https://github.com/omniedgeio/app-release/releases/tag/v0.2.3)


## Cli Command

### Login

- Login By Password

```shell
omniedge login -u xxx@xxx.com
```

-  Login By Secret-Key

You can generate secret-key on omniedge web.

```shell
omniedge login -s xxxxxx
```

### Join

you can just call `omniedge join`, it will automatically prompt 
the available network for you to choose. And you can 
also add one parameter `-n` to specify the network id manually.

And then, enjoy the omniedge network.

```shell
omniedge join 
// or
omniedge join -n "virtual-network-id" 
```

## Protocol

[n2n](https://github.com/ntop/n2n)

## Resources

- Architecture: https://omniedge.io/docs/article/architecture
- Install: https://omniedge.io/docs/article/install
- Cases: https://omniedge.io/docs/article/cases
- Compare: https://omniedge.io/docs/article/compare
- Performance: https://omniedge.io/docs/article/performance
- Dashboard: https://omniedge.io/docs/article/admin
- [n2n](https://github.com/ntop/n2n)


## Contributing Guildlines

Check the tempalte into .github folder to report an issue or submit a PR: 
1. ISSUE_TEMPLATE.md 
2. PULL_REQUEST_TEMPLATE.md 

## How to get started? 

1. If you only need a convenient connectivity service 
Just visit https://omniedge.io/download and download the apps for your platform. 

# Compiler and debugger

`MSVC 2017 32Bit` in Qt Creator

Please change build directory to root/debug and root/release

# Promoting updates

1. Copy all required files to release/packages/<package-name>/data
2. cd to release folder
3. Re-create repo `repogen.exe --update -p packages -i com.omniedge-inc.omniedge-windows repository`
4. Re-create installer `binarycreator.exe -c config\config.xml -p packages Omniedge.exe`
5. cd to repository folder
6. `git push`

# Contributors

@harri8807 @Tex-tang
