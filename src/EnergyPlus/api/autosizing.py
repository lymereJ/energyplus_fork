from ctypes import cdll, c_void_p
from pyenergyplus.common import RealEP


class ThisSizer:
    """
    This sizer class wraps the internal HeatingAirflowUASizer class
    """

    def __init__(self, api: cdll):
        self.api = api
        self.api.sizerNew.argtypes = []
        self.api.sizerNew.restype = c_void_p
        self.api.sizerInitialize.argtypes = [c_void_p, RealEP]
        self.api.sizerInitialize.restype = c_void_p
        self.api.sizerDelete.argtypes = [c_void_p]
        self.api.sizerDelete.restype = c_void_p
        self.api.sizerCalculate.argtypes = [c_void_p]
        self.api.sizerCalculate.restype = int
        self.api.sizerValue.argtypes = [c_void_p]
        self.api.sizerValue.restype = RealEP
        self.instance = self.api.sizerNew()

    def __del__(self):
        self.api.sizerDelete(self.instance)

    def initialize(self, temperature: float) -> None:
        """
        Performs initialization of the sizer, eventually it will have lots of args

        :return: Nothing
        """
        self.api.sizerInitialize(self.instance, temperature)

    def calculate(self) -> bool:
        """
        Performs autosizing calculations with the given initialized values

        :return: True if the sizing was successful, or False if not
        """
        return True if self.api.sizerCalculate(self.instance) == 0 else False

    def autosized_value(self) -> float:
        """
        Returns the autosized value, assuming the calculation was successful

        :return: The autosized value
        """
        return self.api.sizerValue(self.instance)


class Autosizing:
    """
    A wrapper class for all the autosizing classes, acting as a factory
    """

    def __init__(self, api: cdll):
        self.api = api

    def heating_airflow_ua_sizer(self) -> ThisSizer:
        return ThisSizer(self.api)
