from distutils.core import setup, Extension
setup(
    name='winapy', version='0.0.4',
    ext_modules=[
        Extension('winapy_gdi',  ['src\\winapy_gdi\\src.c']),
        Extension('winapy_user', ['src\\winapy_user\\src.c']),
        Extension('winapy_mme',  ['src\\winapy_mme\\src.c'])
    ],
    packages=['src\\winapy', 'src\\winapy_con'],
    author='Itzsten', classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: Microsoft :: Windows",
        "Intended Audience :: Education",
        "Development Status :: 4 - Beta"
    ],
    description='A WinAPI (Windows API) wrapper made in C for Python.',
    long_description="Currently, 135 Windows functions and macros are supported, and there will be more futuristically.\n\nWinAPY features high-level and rich communications with the Windows API, bringing a low-level mess down to a single function. Although it may currently have a few plenties of integrations, this project aims at wrapping as many integrations as possible to Python and will feature more.",
    url="https://github.com/Itzsten/WinAPY",
    author_email="itzsten@gmail.com",
    python_requires='>=3',
    long_description_content_type='text/x-rst'
)
