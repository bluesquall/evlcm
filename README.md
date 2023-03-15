`evlcm` := [`libev`] + [LCM]
============================

An example using [`libev`] and [LCM].

To run with a different LCM provider, set the environment variable
`LCM_DEFAULT_URL`, e.g.:
```
LCM_DEFAULT_URL=udpm://239.255.86.68:8668?ttl=0 ./evlcm -v
```

_____________
[`libev`]: https://github.com/enki/libev
[LCM]: https://github.com/lcm-proj/lcm
