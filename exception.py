class RLPError(Exception):
    pass

class OutOfBoundsError(RLPError):
    pass

class RecursionDepthError(RLPError):
    pass

class GoingInCirclesError(RLPError):
    pass
