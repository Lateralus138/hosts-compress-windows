# Hosts Compress - [hostscompress] (Pre-release)

![Readme Card](https://github-readme-stats.vercel.app/api/pin/?username=Lateralus138&repo=hosts-compress-windows)

---

### Changelog

  - 1.0.0.0 - Initial release.
  - 1.1.0.0 - Fixed regular expression match for URLs that begin with `0\.0\.0\.0\..*` and `127\.0\.0\.1\..*` while still keeping `0.0.0.0 0.0.0.0` and `127.0.0.1 127.0.0.1`.
  - 1.1.1.0 - Bug fix for last entry not being prepended with the correct IP address as reported in issue: [Missing 0.0.0.0 in front of the last line in the output file #1](https://github.com/Lateralus138/hosts-compress-windows/issues/1).
  - 2.0.0.0 - Complete removal of progress bar and any console mode code and therefore considered a completely different version.
    - Progress bar removed because this program is lightning fast even for large files. There's no need for it.
    - Console mode removed because it no longer works well in modern Windows/CMD. I can't imagine a reason for people to be using CMD in this day and age anyway and I no longer feel compelled to cater to it. Having said that, I do still plan to look into possibilities in the future, but it's not a priority.

#### Coming Updates
  - 2.1.0.0 - I will possibly be looking into proper display usage for console mode in modern CMD. This is not a priority and no promise for a time line.

---

This information is generated in a *GitHub Action* immediately after the successful build of this project.

### Current Windows X86 MD5

![WINDOWS X86 MD5](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/Lateralus138/hosts-compress-windows/master/docs/json/hostscompress_x86_md5.json)

### Current Windows X64 MD5

![WINDOWS X64 MD5](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/Lateralus138/hosts-compress-windows/master/docs/json/hostscompress_x64_md5.json)

---

Find the current stable released executables and archived source files here.

For any version of the binary you&#39;ll probably want to change the name of the executable to just&#58; *`hostscompress`*&#46;
