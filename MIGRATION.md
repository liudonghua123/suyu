# Migrating from yuzu

When coming from yuzu, the migration is as easy as renaming some directories.

## Linux

```
 $ mv ~/.local/share/yuzu ~/.local/share/suyu
 $ mv ~/.config/yuzu ~/.config/suyu
```

Depending on your setup, you may want to substitute those base paths for `$XDG_DATA_HOME` and `$XDG_CONFIG_HOME` respectively.
There is also `~/.cache/yuzu`, which you can safely delete. Suyu will build a fresh cache in its own directory.

## Windows

Search `%APPDATA%` for the `yuzu` directories and simply rename those to `suyu`.