#Smush

Smush is a utility to combine disk images into one file. It was created for [Delta](https://github.com/realdelta/delta) in order to use two different unorthodox partition types together.

Smush is incredibly easy. All you need to do is:

```bash
smush [output] [img1] [img2]...
```

It's simple! The MBR from img1 is used, and the rest of the images are stripped of their MBRs and then combined into the output file.

At a later point, Smush will build an MBR partition table.